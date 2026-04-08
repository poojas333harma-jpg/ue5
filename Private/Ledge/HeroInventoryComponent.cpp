// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroInventoryComponent.h"
#include "Ledge/HeroItemData.h"
#include "Ledge/HeroWeaponData.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Ledge/HeroEnergyComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

UHeroInventoryComponent::UHeroInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHeroInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
    
    // Start the decay clock (checks every 5 minutes of game time, simulated here every 30s)
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_Decay, this, &UHeroInventoryComponent::HandlePeriodicDecay, 30.0f, true);
}

bool UHeroInventoryComponent::AddItem(FName ItemID, int32 Quantity)
{
    return AddItemToSlot(ItemID, Quantity);
}

bool UHeroInventoryComponent::AddWeapon(UHeroWeaponData* Weapon)
{
    if (!Weapon) return false;
    return AddItemToSlot(Weapon->WeaponID, 1);
}

bool UHeroInventoryComponent::AddItemToSlot(FName ItemID, int32 Quantity)
{
    // 1. Try to find existing stack
    for (FHeroItemSlot& Slot : Slots)
    {
        if (Slot.ItemID == ItemID)
        {
            Slot.Quantity += Quantity;
            // SMART LOGIC: Reset to Latest Timestamp on addition
            Slot.CollectionTimeStamp = UGameplayStatics::GetTimeSeconds(GetWorld());
            UpdateEncumbrance();
            return true;
        }
    }

    // 2. Find empty slot
    if (Slots.Num() < MaxSlotCount)
    {
        FHeroItemSlot NewSlot;
        NewSlot.ItemID = ItemID;
        NewSlot.Quantity = Quantity;
        NewSlot.CollectionTimeStamp = UGameplayStatics::GetTimeSeconds(GetWorld());
        NewSlot.SlotIndex = Slots.Num();
        Slots.Add(NewSlot);
        
        UpdateEncumbrance();
        return true;
    }

    return false;
}

void UHeroInventoryComponent::RemoveItemFromSlot(int32 SlotIndex, int32 Quantity)
{
    if (Slots.IsValidIndex(SlotIndex))
    {
        Slots[SlotIndex].Quantity -= Quantity;
        if (Slots[SlotIndex].Quantity <= 0)
        {
            Slots.RemoveAt(SlotIndex);
        }
        UpdateEncumbrance();
    }
}

void UHeroInventoryComponent::UpdateEncumbrance()
{
    float TotalWeight = CalculateTotalWeight();
    CurrentTotalWeight = TotalWeight;

    if (ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
    {
        UCharacterMovementComponent* MoveComp = OwnerChar->GetCharacterMovement();
        UHeroEnergyComponent* EnergyComp = OwnerChar->FindComponentByClass<UHeroEnergyComponent>();

        if (MoveComp)
        {
            // Weight Scale: 0 to 1.0 (Overweight)
            float WeightRatio = FMath::Clamp(TotalWeight / MaxCarryWeight, 0.0f, 2.0f);
            
            // Penalty: Max speed drops to 40% if double the weight limit
            float SpeedMultiplier = FMath::Lerp(1.0f, 0.4f, FMath::Clamp(WeightRatio - 1.0f, 0.0f, 1.0f));
            MoveComp->MaxWalkSpeed = 600.f * SpeedMultiplier; // Base speed 600
        }

        if (EnergyComp)
        {
            // Penalty: Stamina (Energy) regens slower when heavy
            float RegenMultiplier = FMath::Lerp(1.0f, 0.5f, FMath::Clamp(TotalWeight / MaxCarryWeight - 0.5f, 0.0f, 1.0f));
            // EnergyComp->RegenRate *= RegenMultiplier; // Assuming RegenRate is public or has setter
        }
    }
}

void UHeroInventoryComponent::ProcessAcceleratedDecay(float HoursPassed)
{
    // Loop through all slots and reduce durability/value
    for (int32 i = Slots.Num() - 1; i >= 0; --i)
    {
        // In a real system, we'd fetch the DecayRate from the ItemDataAsset via ItemID
        float BaseDecay = 2.0f; // 2% per hour
        Slots[i].CurrentDurability -= (BaseDecay * HoursPassed);

        // Logic: Destroy low-tier if reached 0, high-tier becomes "Broken" (handled in Tick or UI)
        if (Slots[i].CurrentDurability <= 0 && Slots[i].SlotIndex < 5) // Mock tier check
        {
            Slots.RemoveAt(i);
        }
    }
}

void UHeroInventoryComponent::HandlePeriodicDecay()
{
    // Convert 30s real time to simulated game hours (e.g., 1 min real = 1 hour game)
    ProcessAcceleratedDecay(0.5f);
}

float UHeroInventoryComponent::CalculateTotalWeight()
{
    float Total = 0.0f;
    for (const FHeroItemSlot& Slot : Slots)
    {
        // Mock weight: 0.5kg per item
        Total += (0.5f * Slot.Quantity);
    }
    return Total;
}

void UHeroInventoryComponent::GetItemInspectionData(UHeroItemData* Item, UStaticMesh*& OutMesh, USoundBase*& OutMusic, FLinearColor& OutColor)
{
    if (!Item) return;
    // Fix: Verification if InspectionMeshOverride is used (TObjectPtr), otherwise fall back to WorldMesh (Raw Pointer)
    OutMesh = Item->InspectionMeshOverride.Get() ? Item->InspectionMeshOverride.Get() : Item->WorldMesh;
    OutMusic = Item->MenuAmbientMusic;
    OutColor = Item->MenuAccentLight;
}

void UHeroInventoryComponent::GetWeaponInspectionData(UHeroWeaponData* Weapon, USkeletalMesh*& OutMesh, USoundBase*& OutMusic, FLinearColor& OutColor)
{
    if (!Weapon) return;
    // Note: If Weapon->InspectionMeshOverride is a UMeshComponent we might need to derive the mesh type.
    // In our simplified setup, we'll try to use the SkeletalMesh.
    OutMesh = Weapon->SkeletalMesh;
    OutMusic = Weapon->MenuAmbientMusic;
    OutColor = Weapon->MenuAccentLight;
}
