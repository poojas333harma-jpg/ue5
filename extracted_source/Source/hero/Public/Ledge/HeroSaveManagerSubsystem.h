// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HeroCoreTypes.h"
#include "HeroSaveManagerSubsystem.generated.h"

class UHeroSaveGame;

/**
 * AAA SAVE MANAGER SUBSYSTEM
 * A global C++ subsystem to handle RPG persistence.
 * Safe, synchronous (or async), and accessible from Blueprints.
 */
UCLASS()
class HERO_API UHeroSaveManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    /** Global Save Trigger (Auto-save on Legendary Loot / Checkpoint) */
    UFUNCTION(BlueprintCallable, Category="HeroSave")
    bool SaveSamratProgress(const FHeroPlayerSaveData& InPlayerData, const TArray<FHeroVehicleSaveData>& InVehicles);

    /** Global Load Trigger */
    UFUNCTION(BlueprintCallable, Category="HeroSave")
    UHeroSaveGame* LoadSamratProgress();

    /** Checks if a save exists */
    UFUNCTION(BlueprintPure, Category="HeroSave")
    bool HasSaveFile() const;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HeroSave")
    FString CurrentSlotName = TEXT("HeroSlot_0");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HeroSave")
    int32 CurrentUserIndex = 0;
};
