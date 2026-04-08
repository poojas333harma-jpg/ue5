// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HeroCoreTypes.h"
#include "HeroWorldDataAsset.generated.h"

/**
 * AAA WEATHER SETTINGS
 * Defines the visual parameters for a specific weather state.
 */
USTRUCT(BlueprintType)
struct FHeroWeatherSettings
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SunIntensity = 10.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor SkyLightColor = FLinearColor(1, 1, 1);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float VolumetricFogDensity = 0.02f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor FogAlbedo = FLinearColor(0.5, 0.5, 0.5);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CloudCoverage = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class USoundBase* AmbientWindSFX;
};

/**
 * AAA WORLD DATA ASSET
 * A master template for defining the 'Living World' environment settings.
 */
UCLASS()
class HERO_API UHeroWorldDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weather")
    TMap<EHeroWeatherType, FHeroWeatherSettings> WeatherPresets;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cycle")
    float DayLengthMinutes = 20.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cycle")
    float NightLengthMinutes = 10.f;
};
