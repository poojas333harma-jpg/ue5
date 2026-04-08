// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HeroCoreTypes.h"
#include "HeroNavigationComponent.generated.h"

/**
 * AAA NAVIGATION & RADAR COMPONENT
 * Handles Mini-map logic, 3D World Markers, and Road-Path rendering.
 * Supports inter-map pathfinding via Teleporters.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HERO_API UHeroNavigationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHeroNavigationComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // ═════════════════════════════════════════════
    // 1. MINI-MAP & RADAR
    // ═════════════════════════════════════════════

    /** Current rotation of the minimap (Degrees) */
    UPROPERTY(BlueprintReadOnly, Category = "Hero|Nav")
    float MinimapRotation;

    /** Dynamic Texture assigned based on current Level Name */
    UPROPERTY(BlueprintReadOnly, Category = "Hero|Nav")
    class UTexture2D* CurrentMapTexture;

    /** --- SKILL-LOCKED HUD (Phase 17) --- */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Nav|Skills")
    bool bIsMiniMapUnlocked = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Nav|Skills")
    bool bIsCompassUnlocked = false;

    /** Updates the minimap icons and rotation based on player heading */
    UFUNCTION(BlueprintCallable, Category = "Hero|Nav")
    void UpdateMinimapLogic();

    /** Swaps the minimap texture based on the current level */
    UFUNCTION(BlueprintCallable, Category = "Hero|Nav")
    void RefreshMapTexture(FName LevelName);

    // ═════════════════════════════════════════════
    // 2. 3D WAYPOINTS & DISTANCE
    // ═════════════════════════════════════════════

    /** Projects a world location to a screen-space 3D marker */
    UFUNCTION(BlueprintCallable, Category = "Hero|Nav")
    void ProjectWorldMarker(FVector WorldLocation, FVector2D& ScreenPosition, float& DistanceMeters);

    /** Current Active Objective Location */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Nav")
    FVector ActiveGoalLocation;

    // ═════════════════════════════════════════════
    // 3. INTER-MAP PATHFINDING
    // ═════════════════════════════════════════════

    /** 
     * Finds the shortest path to a goal, potentially through a Teleporter.
     * @param GoalLocation - Target in any map.
     * @return The location of the nearest transition gate if Goal is in another level.
     */
    UFUNCTION(BlueprintCallable, Category = "Hero|Nav")
    FVector GetCalculatedNavPath(FVector GoalLocation);

    /** Renders the 'Road Path' glowing line for vehicles/mounts */
    UFUNCTION(BlueprintCallable, Category = "Hero|Nav")
    void RenderRoadPath();

private:
    /** Reference to the Teleport Subsystem */
    UPROPERTY()
    class UHeroTeleportManager* TeleportManager;
};
