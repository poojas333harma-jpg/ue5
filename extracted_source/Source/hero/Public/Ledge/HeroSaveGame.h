// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "HeroCoreTypes.h"
#include "HeroSaveGame.generated.h"

/**
 * AAA SAVE GAME OBJECT
 * The master container for all persistent data in the RPG.
 */
UCLASS()
class HERO_API UHeroSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
    UHeroSaveGame();

    UPROPERTY(BlueprintReadWrite, Category="SaveData")
    FString SlotName;

    UPROPERTY(BlueprintReadWrite, Category="SaveData")
    int32 UserIndex;

    // --- Core Data ---
    UPROPERTY(BlueprintReadWrite, Category="SaveData|Player")
    FHeroPlayerSaveData PlayerData;

    UPROPERTY(BlueprintReadWrite, Category="SaveData|World")
    TMap<FName, FHeroVehicleSaveData> VehicleRegistry;

    UPROPERTY(BlueprintReadWrite, Category="SaveData|World")
    EHeroWeatherType LastWeatherType;
};
