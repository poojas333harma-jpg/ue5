// Copyright 2026 Hero Project. All Rights Reserved.

#include "Traffic/HeroTrafficSpline.h"

AHeroTrafficSpline::AHeroTrafficSpline()
{
	PrimaryActorTick.bCanEverTick = false;

	TrafficSpline = CreateDefaultSubobject<USplineComponent>(TEXT("TrafficSpline"));
	RootComponent = TrafficSpline;
}

void AHeroTrafficSpline::BeginPlay()
{
	Super::BeginPlay();
}

void AHeroTrafficSpline::SyncWithZoneGraph()
{
	// Logic to translate Spline points into ZoneGraph Data
	// In a full implementation, this uses MassEntity ZoneGraph subsystem components.
}

FVector AHeroTrafficSpline::GetLaneLocationAtDistance(int32 LaneIndex, float Distance)
{
	if (!TrafficSpline) return GetActorLocation();

	FVector BaseLocation = TrafficSpline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	FVector RightVector = TrafficSpline->GetRightVectorAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);

	// LHD Logic: Left lanes are positive/negative depending on Forward/Right alignment.
	// We offset by LaneIndex * LaneOffset.
	float Multiplier = bIsLeftHandDrive ? -1.0f : 1.0f;
	
	// Offset logic for multiple lanes
	float FinalOffset = (LaneIndex - (LaneCount / 2.0f) + 0.5f) * LaneOffset * Multiplier;

	return BaseLocation + (RightVector * FinalOffset);
}
