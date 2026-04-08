// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroCameraManagerComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Pawn.h"

UHeroCameraManagerComponent::UHeroCameraManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHeroCameraManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHeroCameraManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ActiveSpringArm)
	{
		CurrentArmLength = FMath::FInterpTo(CurrentArmLength, TargetArmLength, DeltaTime, TransitionSpeed);
		ActiveSpringArm->TargetArmLength = CurrentArmLength;
	}
}

void UHeroCameraManagerComponent::TransitionToPawn(APawn* NewPawn)
{
    if (!NewPawn) return;

    ActiveSpringArm = NewPawn->FindComponentByClass<USpringArmComponent>();
    if (ActiveSpringArm)
    {
        CurrentArmLength = ActiveSpringArm->TargetArmLength;
        AdjustSpringArm(NewPawn);
    }
}

void UHeroCameraManagerComponent::AdjustSpringArm(APawn* NewPawn)
{
    if (!ActiveSpringArm) return;

    // SCALING LOGIC
    // Mouse (Small): TargetArmLength = 150.0f
    // Train (Huge): TargetArmLength = 1500.0f
    // Vehicle (Car): TargetArmLength = 600.0f
    // Humanoid: TargetArmLength = 300.0f
    
    // Logic: Use bounding box to determine approximate size
    FVector Min, Max;
    NewPawn->GetActorBounds(false, Min, Max);
    float Radius = Max.Size();

    if (Radius < 50.f) // Chooha
    {
        TargetArmLength = 150.f;
        ActiveSpringArm->ProbeSize = 2.f; // Barium collision
    }
    else if (Radius > 1000.f) // Train / Large Ship
    {
        TargetArmLength = 1500.f;
        ActiveSpringArm->bEnableCameraLag = true;
        ActiveSpringArm->CameraLagSpeed = 3.f;
    }
    else if (Radius > 200.f) // Car
    {
        TargetArmLength = 600.f;
    }
    else // Character
    {
        TargetArmLength = 300.f;
    }
}
