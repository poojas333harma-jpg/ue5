// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Ledge/HeroCoreTypes.h"
#include "HeroEnvironmentSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeatherChanged, EHeroWeatherType, NewWeather);

/**
 * MASTER ENVIRONMENT SUBSYSTEM
 * Central authority for Weather, Realistic Physics (Friction), and Time-of-Day logic.
 */
UCLASS()
class HERO_API UHeroEnvironmentSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    UFUNCTION(BlueprintCallable, Category = "Hero|World")
    void SetWeather(EHeroWeatherType NewWeather);

    UFUNCTION(BlueprintPure, Category = "Hero|World")
    EHeroWeatherType GetCurrentWeather() const { return CurrentWeather; }

    UPROPERTY(BlueprintAssignable, Category = "Hero|World")
    FOnWeatherChanged OnWeatherChanged;

private:
    EHeroWeatherType CurrentWeather = EHeroWeatherType::Clear;

    /** Updates global physics properties (like Friction) based on weather */
    void ApplyWeatherEffects();
};
