// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroRailVehicle.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"

AHeroRailVehicle::AHeroRailVehicle()
{
	PrimaryActorTick.bCanEverTick = true;
    bIsDerailed = false;
}

void AHeroRailVehicle::BeginPlay()
{
	Super::BeginPlay();
}

void AHeroRailVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (bIsDerailed)
    {
        // 1. Friction Decoupling (Slowly stop if disconnected/crashed)
        CurrentSpeed *= RollingFriction;
        if (CurrentSpeed < 10.f) CurrentSpeed = 0.f;
        
        // Apply transform based on momentum (Simple physics simulation)
        AddActorWorldOffset(GetActorForwardVector() * CurrentSpeed * DeltaTime);
        return;
    }

    if (!CurrentTrack) return;

    // 2. Spline Movement
    CurrentSpeed = FMath::FInterpTo(CurrentSpeed, TargetSpeed, DeltaTime, 0.5f);
    CurrentDistance += CurrentSpeed * DeltaTime;

    // 3. Update Transform to Spline
    FVector NewLoc = CurrentTrack->GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);
    FRotator NewRot = CurrentTrack->GetRotationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);
    
    SetActorLocationAndRotation(NewLoc, NewRot);
}

void AHeroRailVehicle::TriggerDerailment()
{
    if (bIsDerailed) return;

    bIsDerailed = true;
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("CRITICAL: Rail Derailment! Survival Mode Engaged."));

    // 1. Trigger the 'Injured' Transition for Samrat
    OnDerailmentCrash.Broadcast(nullptr, this);

    // 2. In a full implementation, we'd toggle Chaos Physics (Geometry Collection) here.
}

void AHeroRailVehicle::DetachBogie(AHeroRailVehicle* Bogie)
{
    if (!Bogie) return;

    Bogie->TargetSpeed = 0.f; // Let friction take over
    Bogie->bIsDerailed = true; // Decouple from spline update
    
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("BOGIE DECOUPLED: Rolling to a stop..."));
}
