// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HeroEnergyComponent.generated.h"

/**
 * AAA ENERGY & RESOURCE COMPONENT
 * Manages Aether Energy consumption, regeneration, and Overheating signals.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HERO_API UHeroEnergyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHeroEnergyComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ═════════════════════════════════════════════
	// 1. RESOURCE STATS
	// ═════════════════════════════════════════════

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Energy")
	float MaxEnergy = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hero|Energy")
	float CurrentEnergy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Energy")
	float RegenRate = 5.f;

	// ═════════════════════════════════════════════
	// 2. CONSUMPTION LOGIC
	// ═════════════════════════════════════════════

	/** Returns true if enough energy is available */
	UFUNCTION(BlueprintCallable, Category = "Hero|Energy")
	bool ConsumeEnergy(float Amount);

	/** Forcefully deplete energy for the "Power Down" crash */
	UFUNCTION(BlueprintCallable, Category = "Hero|Energy")
	void DepleteEnergy();

	/** Signal for Vehicles or Abilities */
	UPROPERTY(BlueprintReadOnly, Category = "Hero|Energy")
	bool bIsOverheated = false;

private:
	void HandleRegeneration(float DeltaTime);
};
