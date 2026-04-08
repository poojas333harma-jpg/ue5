// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Ledge/HeroCoreTypes.h"
#include "HeroGoreComponent.generated.h"

/**
 * ULTRA-REAL DISMEMBERMENT COMPONENT
 * Handles real-time bone hiding, procedural gib spawning, 
 * and persistent injury states (Limping/Blood Trails).
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HERO_API UHeroGoreComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHeroGoreComponent();

	/** 
	 * Main Slicing Logic. Call this from the Weapon's Hit Event.
	 * @param BoneName - The specific bone that was hit.
	 * @param HitForce - Impact intensity.
	 * @param HitNormal - For blood spurt orientation.
	 */
	UFUNCTION(BlueprintCallable, Category = "Hero|Gore")
	void ProcessLimbHit(FName BoneName, float HitForce, FVector HitNormal);

    /** Triggers the Material Dissolve effect after looting is complete. */
    UFUNCTION(BlueprintCallable, Category = "Hero|Gore")
    void StartPostMortemDissolve(float Duration = 5.0f);

protected:
	virtual void BeginPlay() override;

	/** Spawns a physicalized limb actor and hides the bone on the owner's mesh. */
	void DismemberLimb(FName BoneName, FVector Impulse);

	/** Activates the persistent blood trail for leg injuries. */
	void StartPersistentBloodTrail();

private:
	UPROPERTY()
	class USkeletalMeshComponent* OwnerMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Hero|Gore")
	TArray<FHeroGoreBoneData> SupportBones;

    /** Dissolve Timeline / Parameter logic */
    float CurrentDissolve = 0.f;
    bool bIsDissolving = false;

    /** Niagara Components for Blood */
    UPROPERTY()
    class UNiagaraComponent* ActiveBloodTrail;
};
