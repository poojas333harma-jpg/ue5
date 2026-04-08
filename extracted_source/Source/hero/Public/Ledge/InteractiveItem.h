// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HeroCoreTypes.h"
#include "InteractiveItem.generated.h"

class UCapsuleComponent;
class UStaticMeshComponent;
class UHeroOverlayData;

/**
 * AAA INTERACTIVE ITEM
 * Base class for Pickups, Heavy Objects, and Contextual Interactions.
 */
UCLASS(Blueprintable, meta=(DisplayName="Hero Interactive Item"))
class HERO_API AInteractiveItem : public AActor
{
    GENERATED_BODY()

public:
    AInteractiveItem();

protected:
    virtual void BeginPlay() override;

public:
    // --- 1. Components ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interaction")
    UCapsuleComponent* TriggerZone;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interaction")
    UStaticMeshComponent* ItemMesh;

    // --- 2. Pickup Logic ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction|Data")
    EHeroPickupType InteractionType = EHeroPickupType::Quick;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction|Data")
    class UHeroWeaponData* WeaponData; // If this is a weapon

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction|Data")
    class UHeroItemData* ItemData; // If this is a generic item

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction|Data")
    class UHeroOverlayData* ResultOverlay; // The overlay to apply after pickup (if not using WeaponData)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction|Data")
    bool bDestroyOnPickup = true;

    // --- 3. Animation & Warping ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction|Anim")
    UAnimMontage* InteractionMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction|Anim")
    FName WarpTargetName = FName("PickupTarget");

    // --- 4. Logic Functions ---
    UFUNCTION(BlueprintCallable, Category="Interaction")
    virtual bool ExecuteInteraction(AActor* Interactor);

    // --- 5. Overlap Events ---
    UFUNCTION()
    virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
                                bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
