// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/AHeroWeatherManager.h"
#include "Ledge/HeroWorldDataAsset.h"

AHeroWeatherManager::AHeroWeatherManager()
{
	PrimaryActorTick.bCanEverTick = true;
    CurrentWeather = EHeroWeatherType::Clear;
}

void AHeroWeatherManager::BeginPlay()
{
	Super::BeginPlay();
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("WORLD DIRECTOR ACTIVE: Day/Night Cycle Engaged"));
}

void AHeroWeatherManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // 1. Smoothly update Master Intensity (Interpolation Logic)
    // Scale intensity towards 1.0 during active weather events (Storm, Fog)
    if (CurrentWeather != EHeroWeatherType::Clear)
    {
        MasterWeatherIntensity = FMath::FInterpTo(MasterWeatherIntensity, 1.0f, DeltaTime, 0.5f);
    }
    else
    {
        MasterWeatherIntensity = FMath::FInterpTo(MasterWeatherIntensity, 0.0f, DeltaTime, 0.5f);
    }

    // 2. Broadcast to Blueprint for Post-Process Material update (Lens Distortion, Glitch)
    OnWeatherTransitionUpdated(MasterWeatherIntensity, CurrentWeather);

    // 3. Optional: Randomize weather based on WorldData Timer
    /*
    if (bUseTimerForCycle)
    {
        // Cycle logic...
    }
    */
}

void AHeroWeatherManager::SetTargetWeather(EHeroWeatherType NewType, float TransitionDuration)
{
    CurrentWeather = NewType;
    
    FString WeatherStr = "";
    switch (NewType)
    {
        case EHeroWeatherType::Storm: WeatherStr = "THUNDERSTORM"; break;
        case EHeroWeatherType::RadioactiveFog: WeatherStr = "RADIOACTIVE FOG (Dying Light)"; break;
        case EHeroWeatherType::Clear: WeatherStr = "CLEAR SKIES"; break;
    }

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, 
        FString::Printf(TEXT("STORY OVERRIDE: Weather Shifting to %s..."), *WeatherStr));
}
