// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HeroLightSensorComponent.generated.h"

/**
 * PHASE 58: STEALTH & AGGRESSION - HIDDEN LIGHT SENSOR
 * Samples environmental lighting to produce a Visibility Scalar (0.0 - 1.0).
 * This scalar modulates NPC detection radius in real-time.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HERO_API UHeroLightSensorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHeroLightSensorComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    /** Current visibility intensity (0.0 = total darkness, 1.0 = bright light/sunlight) */
    UPROPERTY(BlueprintReadOnly, Category = "Stealth")
    float VisibilityScalar;

    /** How often to sample the light (optimization) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stealth")
    float UpdateFrequency = 0.1f;

private:
    float TimeSinceLastUpdate = 0.0f;

    /** 
     * Performs the light sampling logic.
     * In UE5, this can be done via line traces to light sources or by sampling the GPU exposure.
     * We'll implement a fast multi-point light probe.
     */
    void CalculateVisibility();
	
};
