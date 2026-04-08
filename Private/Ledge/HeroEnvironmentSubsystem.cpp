// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroEnvironmentSubsystem.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EngineUtils.h"

void UHeroEnvironmentSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UHeroEnvironmentSubsystem::SetWeather(EHeroWeatherType NewWeather)
{
    if (CurrentWeather != NewWeather)
    {
        CurrentWeather = NewWeather;
        ApplyWeatherEffects();
        OnWeatherChanged.Broadcast(CurrentWeather);
    }
}

void UHeroEnvironmentSubsystem::ApplyWeatherEffects()
{
    // Iterate over all characters in the world to modify physics friction
    for (TActorIterator<ACharacter> It(GetWorld()); It; ++It)
    {
        ACharacter* Char = *It;
        if (Char && Char->GetCharacterMovement())
        {
            if (CurrentWeather == EHeroWeatherType::Rain || CurrentWeather == EHeroWeatherType::Storm)
            {
                // REDUCE FRICTION: Makes turning slippery
                Char->GetCharacterMovement()->GroundFriction = 4.0f; // Default is usually 8.0f
                Char->GetCharacterMovement()->BrakingDecelerationWalking = 1000.0f; // Default is ~2000.0f
            }
            else
            {
                // RESTORE DEFAULT
                Char->GetCharacterMovement()->GroundFriction = 8.0f;
                Char->GetCharacterMovement()->BrakingDecelerationWalking = 2048.0f;
            }
        }
    }
}
