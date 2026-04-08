// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroSaveManagerSubsystem.h"
#include "Ledge/HeroSaveGame.h"
#include "Kismet/GameplayStatics.h"

void UHeroSaveManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

bool UHeroSaveManagerSubsystem::SaveSamratProgress(const FHeroPlayerSaveData& InPlayerData, const TArray<FHeroVehicleSaveData>& InVehicles)
{
    // 1. Load or Create Save Object
    UHeroSaveGame* SaveObj = Cast<UHeroSaveGame>(UGameplayStatics::LoadGameFromSlot(CurrentSlotName, CurrentUserIndex));
    if (!SaveObj)
    {
        SaveObj = Cast<UHeroSaveGame>(UGameplayStatics::CreateSaveGameObject(UHeroSaveGame::StaticClass()));
    }

    if (!SaveObj) return false;

    // 2. Sync Samrat's Data
    SaveObj->PlayerData = InPlayerData;

    // 3. Register Vehicles (Sync punctures, etc.)
    for (const FHeroVehicleSaveData& VData : InVehicles)
    {
        SaveObj->VehicleRegistry.FindOrAdd(VData.VehicleID) = VData;
    }

    // 4. Persistence to Disk
    bool bSuccess = UGameplayStatics::SaveGameToSlot(SaveObj, CurrentSlotName, CurrentUserIndex);
    
    if (bSuccess)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("AUTO-SAVE: Samrat's Progression and Vehicle State Secured!"));
    }

    return bSuccess;
}

UHeroSaveGame* UHeroSaveManagerSubsystem::LoadSamratProgress()
{
    if (!UGameplayStatics::DoesSaveGameExist(CurrentSlotName, CurrentUserIndex)) return nullptr;

    UHeroSaveGame* LoadedSave = Cast<UHeroSaveGame>(UGameplayStatics::LoadGameFromSlot(CurrentSlotName, CurrentUserIndex));
    
    if (LoadedSave)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("LOADING: Hero RPG Framework - Samrat's Data Restored."));
    }

    return LoadedSave;
}

bool UHeroSaveManagerSubsystem::HasSaveFile() const
{
    return UGameplayStatics::DoesSaveGameExist(CurrentSlotName, CurrentUserIndex);
}
