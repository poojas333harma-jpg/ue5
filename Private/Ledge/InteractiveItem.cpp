// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/InteractiveItem.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "MotionWarpingComponent.h"
#include "Ledge/HeroOverlayData.h"
#include "Ledge/HeroWeaponData.h"
#include "Ledge/HeroItemData.h"
#include "Ledge/LedgeDetectorComponent.h"
#include "Ledge/HeroInventoryComponent.h"
#include "Ledge/HeroNPCBase.h"

AInteractiveItem::AInteractiveItem()
{
    PrimaryActorTick.bCanEverTick = false;

    // 1. Trigger Zone
    TriggerZone = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TriggerZone"));
    RootComponent = TriggerZone;
    TriggerZone->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    TriggerZone->SetCapsuleSize(100.f, 100.f);

    // 2. Mesh
    ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
    ItemMesh->SetupAttachment(RootComponent);
    ItemMesh->SetCollisionProfileName(TEXT("NoCollision"));

    // 3. Bind Events
    TriggerZone->OnComponentBeginOverlap.AddDynamic(this, &AInteractiveItem::OnOverlapBegin);
    TriggerZone->OnComponentEndOverlap.AddDynamic(this, &AInteractiveItem::OnOverlapEnd);
}

void AInteractiveItem::BeginPlay()
{
    Super::BeginPlay();
}

void AInteractiveItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
                                     bool bFromSweep, const FHitResult& SweepResult)
{
    ACharacter* Char = Cast<ACharacter>(OtherActor);
    if (Char && Char->IsPlayerControlled())
    {
        // TODO: UI Notification "Press E to Pickup"
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("Press E to Interact"));
    }
}

void AInteractiveItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    // Clear UI etc.
}

bool AInteractiveItem::ExecuteInteraction(AActor* Interactor)
{
    ACharacter* Char = Cast<ACharacter>(Interactor);
    if (!Char || !InteractionMontage) return false;

    // 1. Motion Warping (Sync character to item)
    UMotionWarpingComponent* Warper = Char->FindComponentByClass<UMotionWarpingComponent>();
    if (Warper)
    {
        // Add or Update Warp Target
        Warper->AddOrUpdateWarpTargetFromLocationAndRotation(WarpTargetName, GetActorLocation(), GetActorRotation());
    }

    // 2. Play Interaction Montage
    Char->PlayAnimMontage(InteractionMontage);

    // 3. Inventory Integration
    UHeroInventoryComponent* Inventory = Char->FindComponentByClass<UHeroInventoryComponent>();
    if (Inventory)
    {
        if (WeaponData) Inventory->AddWeapon(WeaponData);
        if (ItemData) Inventory->AddItemToSlot(ItemData->ItemID, 1);
    }

    // 4. Handle Physics / Overlay
    // If we have weapon data, use its overlay. Otherwise fallback to ResultOverlay.
    EHeroOverlayState NewOverlayState = EHeroOverlayState::HERO_OVERLAY_NONE;
    
    if (WeaponData)
    {
        NewOverlayState = WeaponData->TargetOverlayState;
    }
    else if (ResultOverlay)
    {
        NewOverlayState = ResultOverlay->OverlayState;
    }

    if (NewOverlayState != EHeroOverlayState::HERO_OVERLAY_NONE)
    {
        ULedgeDetectorComponent* LedgeComp = Char->FindComponentByClass<ULedgeDetectorComponent>();
        if (LedgeComp)
        {
            LedgeComp->SetOverlayState(NewOverlayState);
        }
    }
    
    if (bDestroyOnPickup)
    {
        SetLifeSpan(0.1f); // Quick destroy
    }

    return true;
}
