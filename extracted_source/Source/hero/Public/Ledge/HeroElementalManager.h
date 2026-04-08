// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Ledge/HeroCoreTypes.h"
#include "HeroElementalManager.generated.h"

/**
 * MASTER ELEMENTAL MANAGER
 * Processes Chemistry-Engine interactions (Electric + Water, Fire Propagation, Oil + Explosive).
 */
UCLASS()
class HERO_API UHeroElementalManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
    /** 
     * Triggers an electrical discharge in water. 
     * @param Location - Impact Point.
     * @param Radius - Conductivity Range (e.g. 600.0f).
     */
    UFUNCTION(BlueprintCallable, Category = "Hero|Elemental")
    void ExecuteElectricalDischargeInWater(FVector Location, float Radius = 600.0f);

    /** Spreads fire to nearby flammable actors. */
    UFUNCTION(BlueprintCallable, Category = "Hero|Elemental")
    void SpreadFire(FVector Location, float PropagationRadius = 300.0f);

private:
    void ApplyStunAndDamage(AActor* Target, float Duration = 3.0f);
    
    /** Jumps current between Pawns in close proximity */
    void HandleChainReaction(FVector SourceLocation, TArray<AActor*> IgnoredActors);
};
