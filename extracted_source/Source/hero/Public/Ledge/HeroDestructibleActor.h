// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GeometryCollection/GeometryCollectionActor.h"
#include "HeroCoreTypes.h"
#include "HeroDestructibleActor.generated.h"

/**
 * AAA DESTRUCTIBLE ACTOR (CHAOS)
 * Base class for all environmental props (Poles, Hydrants, Fences).
 * Features: Material-based VFX, Strategic Damage Zones, and Persistence.
 */
UCLASS()
class HERO_API AHeroDestructibleActor : public AGeometryCollectionActor
{
	GENERATED_BODY()

public:
	AHeroDestructibleActor();

protected:
	virtual void BeginPlay() override;

public:
    // ═════════════════════════════════════════════
    // 1. CONFIGURATION
    // ═════════════════════════════════════════════
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HeroDestruction")
    EHeroDestructionType PropType = EHeroDestructionType::General;

    /** If true, this actor's destroyed state is saved to FHeroPlayerSaveData */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HeroDestruction")
    bool bIsPersistent = false;

    /** Multiplier for Void-Drive (Humanity Gravity) collisions */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HeroDestruction")
    float HumanityGravityMultiplier = 3.0f;

    // ═════════════════════════════════════════════
    // 2. VFX & SFX
    // ═════════════════════════════════════════════
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HeroDestruction|Effects")
    class UNiagaraSystem* FractureVFX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HeroDestruction|Effects")
    class USoundBase* FractureSFX;

    // ═════════════════════════════════════════════
    // 3. LOGIC MODULES
    // ═════════════════════════════════════════════
    
    UFUNCTION(BlueprintCallable, Category="HeroDestruction")
    void HandleFractureEvent(const FVector& HitLocation, const FVector& HitImpulse);

protected:
    void SpawnHydrantSplash();
    void SpawnPoleElectricZone();
    
    FTimerHandle ResetTimerHandle;
    void StopHydrantWater();
};
