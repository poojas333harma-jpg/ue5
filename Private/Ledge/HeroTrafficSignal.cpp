// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroTrafficSignal.h"
#include "TimerManager.h"

AHeroTrafficSignal::AHeroTrafficSignal()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHeroTrafficSignal::BeginPlay()
{
	Super::BeginPlay();
	StateTimer = SignalSettings.RedDuration; // Start at Red
}

void AHeroTrafficSignal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateSignalCycle(DeltaTime);
}

void AHeroTrafficSignal::UpdateSignalCycle(float DeltaTime)
{
	StateTimer -= DeltaTime;

	if (StateTimer <= 0.f)
	{
		// Transition Logic
		switch (CurrentState)
		{
		case EHeroTrafficState::TS_Red:
			CurrentState = EHeroTrafficState::TS_Green;
			StateTimer = SignalSettings.GreenDuration;
			break;
		case EHeroTrafficState::TS_Green:
			CurrentState = EHeroTrafficState::TS_Yellow;
			StateTimer = SignalSettings.YellowDuration;
			break;
		case EHeroTrafficState::TS_Yellow:
			CurrentState = EHeroTrafficState::TS_Red;
			StateTimer = SignalSettings.RedDuration;
			break;
		}

        BroadcastSignalState();
	}
}

void AHeroTrafficSignal::BroadcastSignalState()
{
    // Global broadcasting logic for Phase 19 (Performance optimization)
    // To minimize CPU cost for 5000 NPCs, we broadcast only on state change.
    UE_LOG(LogTemp, Warning, TEXT("TRAFFIC SIGNAL: %s"), 
        CurrentState == EHeroTrafficState::TS_Green ? TEXT("GREEN") :
        CurrentState == EHeroTrafficState::TS_Red ? TEXT("RED") : TEXT("YELLOW"));
}

// --- PATH MANAGER IMPLEMENTATION ---

#include "Ledge/HeroPathManager.h"

UHeroLaneComponent::UHeroLaneComponent()
{
	bIsLeftHandLane = true;
	SpeedLimit = 40.f;
}

AHeroPathManager::AHeroPathManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AHeroPathManager::BeginPlay()
{
	Super::BeginPlay();
	DiscoverLanes();
}

void AHeroPathManager::DiscoverLanes()
{
	Lanes.Empty();
	TArray<USceneComponent*> ChildComponents;
	GetRootComponent()->GetChildrenComponents(true, ChildComponents);

	for (USceneComponent* Child : ChildComponents)
	{
		if (UHeroLaneComponent* Lane = Cast<UHeroLaneComponent>(Child))
		{
			Lanes.Add(Lane);
		}
	}
}

void AHeroPathManager::InvertLanes()
{
    for (UHeroLaneComponent* Lane : Lanes)
    {
        Lane->bIsLeftHandLane = !Lane->bIsLeftHandLane;
    }
}

FVector AHeroPathManager::GetNearestLanePoint(FVector SourceLocation, bool bPreferLeftHand)
{
    float MinDist = MAX_FLT;
    FVector BestPoint = SourceLocation;

    for (UHeroLaneComponent* Lane : Lanes)
    {
        if (Lane->bIsLeftHandLane == bPreferLeftHand)
        {
            FVector Point = Lane->FindLocationClosestToWorldLocation(SourceLocation, ESplineCoordinateSpace::World);
            float DistSqr = FVector::DistSquared(SourceLocation, Point);
            if (DistSqr < MinDist)
            {
                MinDist = DistSqr;
                BestPoint = Point;
            }
        }
    }
    return BestPoint;
}
