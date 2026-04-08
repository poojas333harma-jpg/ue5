// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroTeleportManager.h"
#include "Kismet/GameplayStatics.h"
#include "Ledge/HeroPlayerController.h"
#include "Engine/Texture2D.h"
#include "Engine/World.h"

void UHeroTeleportManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
    // 1. Initial Cache Setup (Placeholder logic)
    // We would typically load a DataAsset here that contains all Level -> Texture maps.
}

void UHeroTeleportManager::Deinitialize()
{
	Super::Deinitialize();
}

void UHeroTeleportManager::TeleportAcrossLevels(FName TargetMapName, FName TargetGateID)
{
    // DIGITAL DISSOLVE LOGIC (Checklist Phase 14-C)
    PlayTransitionVFX();

    // 1. DYNAMIC LEVEL LOADING: Use UGameplayStatics::OpenLevel
    // This triggers a full level swap. For seamless streaming, we'd use LoadStreamLevel.
    UGameplayStatics::OpenLevel(GetWorld(), TargetMapName);

    // 2. SPAWN SYNC: PC will handle spawning at the correct GateID on BeginPlay of the next level.
}

UTexture2D* UHeroTeleportManager::GetMapTextureForCurrentLevel()
{
    // MAP SWAPPING (Optimization Phase 14-C)
    // Synchronize UI to current ULevelStreaming state.
    FName CurrentLevelName = *GetWorld()->GetMapName();
    
    if (LevelMapCache.Contains(CurrentLevelName))
    {
        return LevelMapCache[CurrentLevelName];
    }

    return nullptr; // Default placeholder for the Minimap UI.
}

FVector UHeroTeleportManager::GetNearestGateToMap(FName TargetMapName)
{
    // BRIDGE LOGIC: Find an actor tagged with 'Gate' and 'TargetMap' matching input.
    // This allows the Navigation Component to route you to the transition point.
    return FVector::ZeroVector; 
}

void UHeroTeleportManager::PlayTransitionVFX()
{
    // Trigger 'Void Portal' fullscreen UI fade or Digital Dissolve effect.
    UE_LOG(LogTemp, Warning, TEXT("[HeroTeleport] Playing Void Portal Transition..."));
}
