// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HeroCoreTypes.h"
#include "Ledge/HeroItemData.h"
#include "Ledge/HeroWeaponData.h"
#include "HeroInventoryComponent.generated.h"

class UHeroItemData;
class UHeroWeaponData;

/**
 * AAA INVENTORY COMPONENT (Dying Light 2 Inspired)
 * Manages picking up, storing, and inspecting items in 3D.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), meta=(DisplayName="Hero Inventory Component"))
class HERO_API UHeroInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHeroInventoryComponent();

protected:
    virtual void BeginPlay() override;

public:
    // ═════════════════════════════════════════════
    // 1. STORAGE (SLOT-BASED GRID)
    // ═════════════════════════════════════════════
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory")
    TArray<FHeroItemSlot> Slots;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory|Config")
    int32 MaxSlotCount = 20;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory|Config")
    float MaxCarryWeight = 50.0f;

    UPROPERTY(BlueprintReadOnly, Category="Inventory|Stats")
    float CurrentTotalWeight = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category="Inventory|Stats")
    int32 CurrentCurrency = 0;

    // ═════════════════════════════════════════════
    // 2. CORE LOGIC
    // ═════════════════════════════════════════════
    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool AddItemToSlot(FName ItemID, int32 Quantity);

    UFUNCTION(BlueprintCallable, Category="Inventory")
    void RemoveItemFromSlot(int32 SlotIndex, int32 Quantity);

    /** Recalculates weight and applies speed/stamina penalties to owner */
    UFUNCTION(BlueprintCallable, Category="Inventory|Logic")
    void UpdateEncumbrance();

    /** DEPRECATED: Use AddItemToSlot. Kept for legacy compatibility. */
    UFUNCTION(BlueprintCallable, Category="Inventory|Legacy")
    bool AddItem(FName ItemID, int32 Quantity);

    /** DEPRECATED: Use AddItemToSlot. Kept for legacy compatibility. */
    UFUNCTION(BlueprintCallable, Category="Inventory|Legacy")
    bool AddWeapon(class UHeroWeaponData* Weapon);

    /** Accelerated Decay for Sleep/FastTravel */
    void ProcessAcceleratedDecay(float HoursPassed);

    // ═════════════════════════════════════════════
    // 3. UI & INSPECTION HOOKS
    // ═════════════════════════════════════════════
    
    /** Returns data needed for the DL2-style 3D Inspection menu */
    UFUNCTION(BlueprintCallable, Category="Inventory|Inspection")
    void GetItemInspectionData(UHeroItemData* Item, UStaticMesh*& OutMesh, class USoundBase*& OutMusic, FLinearColor& OutColor);

    UFUNCTION(BlueprintCallable, Category="Inventory|Inspection")
    void GetWeaponInspectionData(UHeroWeaponData* Weapon, USkeletalMesh*& OutMesh, class USoundBase*& OutMusic, FLinearColor& OutColor);

private:
    void HandlePeriodicDecay();
    float CalculateTotalWeight();

    FTimerHandle TimerHandle_Decay;
};
