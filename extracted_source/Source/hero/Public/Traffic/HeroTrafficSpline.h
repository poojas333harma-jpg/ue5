// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "HeroTrafficSpline.generated.h"

/**
 * AAA TRAFFIC & NAVIGATION SPLINE
 * Handles Left-Hand Drive (LHD) lane offsets and ZoneGraph synchronization.
 * Supports high-density vehicle and pedestrian pathing.
 */
UCLASS()
class HERO_API AHeroTrafficSpline : public AActor
{
	GENERATED_BODY()
	
public:	
	AHeroTrafficSpline();

protected:
	virtual void BeginPlay() override;

public:	
	// ═════════════════════════════════════════════
	// 1. SPLINE CONFIGURATION
	// ═════════════════════════════════════════════

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Traffic|Components")
	USplineComponent* TrafficSpline;

	/** If true, uses Left-Hand Drive (LHD) rules for lane offsets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic|Settings")
	bool bIsLeftHandDrive = true;

	/** Distance between lanes (standard: 350.0f) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic|Settings")
	float LaneOffset = 350.f;

    /** Number of lanes on this spline segment */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic|Settings")
	int32 LaneCount = 2;

	// ═════════════════════════════════════════════
	// 2. ZONEGRAPH SYNCHRONIZATION
	// ═════════════════════════════════════════════

	/** Synchronizes spline data with the Mass Entity ZoneGraph subsystem */
	UFUNCTION(BlueprintCallable, Category = "Traffic|ZoneGraph")
	void SyncWithZoneGraph();

	/** Gets a world-space location for a specific lane index at a distance along spline */
	UFUNCTION(BlueprintPure, Category = "Traffic|Navigation")
	FVector GetLaneLocationAtDistance(int32 LaneIndex, float Distance);
};
