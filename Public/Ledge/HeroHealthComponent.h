// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HeroCoreTypes.h"
#include "HeroHealthComponent.generated.h"

/**
 * AAA REGIONAL HEALTH COMPONENT
 * Handles Limb-specific damage, automatic limping, and gore effects.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDamageTaken, float, Damage, FVector, HitLocation);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HERO_API UHeroHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHeroHealthComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// --- 1. CORE HEALTH ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Health")
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hero|Health")
	float CurrentHealth = 100.f;

    // --- 2. REGIONAL DAMAGE ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Health")
	TMap<FName, float> BodyPartHealth; // BoneName -> Health

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Health")
	TMap<FName, float> BodyPartMultipliers; // BoneName -> Multiplier (Head = 2.0x)

	UFUNCTION(BlueprintCallable, Category = "Hero|Health")
	void ApplyRegionalDamage(float Damage, FName BoneName, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category = "Hero|Health")
	void ApplyEnvironmentalDamage(float DamagePerSecond, float DeltaTime);

	// --- 3. STATE & GORE ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hero|Health")
	bool bIsLimping = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hero|Health")
	bool bIsDead = false;

	/** Spawns blood on the floor based on impact location */
	void SpawnBloodDecal(FVector Location, FVector Normal);

	/** --- DAMAGE EVENT (Phase 17) --- */
	UPROPERTY(BlueprintAssignable, Category = "Hero|Health")
	FOnDamageTaken OnDamageTaken;

	UPROPERTY(EditDefaultsOnly, Category = "Hero|Health")
	class UMaterialInterface* BloodDecalMaterial;

private:
	/** Checks limb health and triggers posture changes (Limping) */
	void CheckLimbCondition();
};
