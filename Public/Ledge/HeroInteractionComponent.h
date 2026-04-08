// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Ledge/HeroCoreTypes.h"
#include "HeroInteractionComponent.generated.h"

/**
 * SAK MASTER INTERACTION COMPONENT
 * Handles Dodge Rolls, Crafting, Lockpicking, and Physical Interactions.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HERO_API UHeroInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHeroInteractionComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ═════════════════════════════════════════════
	// 1. DODGE & INTERACTION
	// ═════════════════════════════════════════════

	UFUNCTION(BlueprintCallable, Category = "Hero|Interaction")
	void TraceForInteractables();

	UFUNCTION(BlueprintCallable, Category = "Hero|Interaction")
	void StartLockpick(EHeroLockDifficulty Difficulty);

	UFUNCTION(BlueprintCallable, Category = "Hero|Interaction")
	void UpdateLockpick(float PinAngle, float BarrelRotation, float DeltaTime, bool& bSuccess, bool& bPinSnapped);

	UFUNCTION(BlueprintCallable, Category = "Hero|Interaction")
	void StartElectronicHack(EHeroLockDifficulty Difficulty);

	UFUNCTION(BlueprintCallable, Category = "Hero|Interaction")
	void UpdateHackPuzzle(int32 NodeIndex, bool& bLevelComplete);

    float GetSweetSpotSize() const;

	UFUNCTION(BlueprintCallable, Category = "Hero|Interaction")
	void StartSearchNPC(class AHeroNPCBase* TargetNPC);

    /** Soft Reference to the Master Loot Actor to keep memory usage low. */
    UPROPERTY(EditDefaultsOnly, Category = "Hero|Loot")
    TSoftClassPtr<AActor> MasterPickupClass;

	UPROPERTY(BlueprintAssignable, Category = "Hero|Interaction")
	FOnInteractionSuccess OnInteractionSuccess;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Interaction")
	float InteractionRange = 250.f;

private:
	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	AActor* LastInteractedActor;

	EHeroLockDifficulty CurrentDifficulty;
	float CurrentPinDurability = 100.f;
	float TargetSweetSpotAngle = 0.f;

	void UpdateDodgeState(float DeltaTime);

	UPROPERTY()
	class UHeroEnergyComponent* EnergyComp;
};

