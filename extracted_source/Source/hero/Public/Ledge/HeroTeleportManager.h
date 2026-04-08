// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "HeroTeleportManager.generated.h"

/**
 * AAA GLOBAL TELEPORT MANAGER (Subsystem)
 * Handles player transitions between sub-levels (Map 1, 2, 3).
 * Manages dynamic map loading and UI synchronization.
 */
UCLASS()
class HERO_API UHeroTeleportManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	/** 
	 * Teleports the player to a specific Gate in a Target Level.
	 * @param TargetMapName - The LevelAsset to load.
	 * @param TargetGateID - The unique ID of the Gate in the target level.
	 */
	UFUNCTION(BlueprintCallable, Category = "Hero|Teleport")
	void TeleportAcrossLevels(FName TargetMapName, FName TargetGateID);

	/**
	 * Returns the Texture associated with the current level for the Mini-Map.
	 */
	UFUNCTION(BlueprintCallable, Category = "Hero|Map")
	class UTexture2D* GetMapTextureForCurrentLevel();

    /**
     * Logic to find the nearest Teleporter leading to a specific Target map.
     */
    UFUNCTION(BlueprintCallable, Category = "Hero|Nav")
    FVector GetNearestGateToMap(FName TargetMapName);

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	/** Stored mapping of LevelNames to Mini-Map Texture Assets */
	UPROPERTY()
	TMap<FName, class UTexture2D*> LevelMapCache;

    /** Internal helper to trigger the 'Void Portal' UI Transition */
    void PlayTransitionVFX();
};
