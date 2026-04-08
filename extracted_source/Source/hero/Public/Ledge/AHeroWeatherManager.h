// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HeroCoreTypes.h"
#include "AHeroWeatherManager.generated.h"

class UHeroWorldDataAsset;

/**
 * AAA WEATHER MANAGER
 * The director for Day/Night cycles, dynamic fog, and storm transitions.
 * Supports smooth transitions between Clear, Storm, and Radioactive Fog.
 */
UCLASS()
class HERO_API AHeroWeatherManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AHeroWeatherManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

    // ═════════════════════════════════════════════
    // 1. ENVIRONMENT CONFIG
    // ═════════════════════════════════════════════
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HeroWorld")
    TObjectPtr<UHeroWorldDataAsset> WorldData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HeroWorld")
    EHeroWeatherType CurrentWeather = EHeroWeatherType::Clear;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HeroWorld")
    bool bUseTimerForCycle = true;

    // ═════════════════════════════════════════════
    // 2. TRANSITION LOGIC
    // ═════════════════════════════════════════════
    
    /** Transitions to a specific weather (e.g. Sudarshan Chakra Storm) */
    UFUNCTION(BlueprintCallable, Category="HeroWorld")
    void SetTargetWeather(EHeroWeatherType NewType, float TransitionDuration = 5.f);

    UPROPERTY(BlueprintReadOnly, Category="HeroWorld")
    float MasterWeatherIntensity = 0.f; // 0.0 to 1.0

    // ═════════════════════════════════════════════
    // 3. BLUEPRINT HOOKS (BP PostProcess / Cinematic)
    // ═════════════════════════════════════════════
    
    /** Hook for the BP-based PostProcess Material (Glitch/Bloom) */
    UFUNCTION(BlueprintImplementableEvent, Category="HeroWorld")
    void OnWeatherTransitionUpdated(float Intensity, EHeroWeatherType WeatherType);

    UFUNCTION(BlueprintImplementableEvent, Category="HeroWorld")
    void OnNightFallStarted();
};
