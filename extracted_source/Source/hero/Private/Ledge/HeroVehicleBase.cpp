// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroVehicleBase.h"
#include "Ledge/HeroVehicleData.h"
#include "Ledge/HeroVehicleImpostor.h"
#include "Ledge/HeroDrivingSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraShakeBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "WorldCollision.h"
#include "Engine/OverlapResult.h"
#include "Ledge/HeroCoreTypes.h"
#include "Ledge/HeroVehicleData.h"
#include "Ledge/HeroVehicleBase.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"





AHeroVehicleBase::AHeroVehicleBase()
{
    PrimaryActorTick.bCanEverTick = true;

    // 1. Audio Components
    TrailerConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("TrailerConstraint"));
    TrailerConstraint->SetupAttachment(GetRootComponent());

    // --- Phase 19-D: Symmetry Components ---
    DriverSeat = CreateDefaultSubobject<USceneComponent>(TEXT("DriverSeat"));
    DriverSeat->SetupAttachment(GetRootComponent());

    SteeringWheel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SteeringWheel"));
    SteeringWheel->SetupAttachment(GetRootComponent());

    EngineAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("EngineAudio"));
    EngineAudio->SetupAttachment(GetMesh());
    
    VoidHumAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("VoidHumAudio"));
    VoidHumAudio->SetupAttachment(GetMesh());
    VoidHumAudio->bAutoActivate = false;

    // 2. VFX
    VoidDriveVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VoidDriveVFX"));
    VoidDriveVFX->SetupAttachment(GetMesh());
    VoidDriveVFX->bAutoActivate = false;
}

void AHeroVehicleBase::BeginPlay()
{
	Super::BeginPlay();

    // --- Phase 18: Physical Calibration ---
    if (VehicleData)
    {
        // 1. Scaled Mass (1,200kg to 10,000kg)
        GetMesh()->SetMassScale(NAME_None, VehicleData->Stats.Mass / 1500.f); // Baseline normalization
        
        // 2. Center of Mass Adjustment
        GetMesh()->SetCenterOfMass(VehicleData->Stats.CenterOfMassOffset);
        
        UE_LOG(LogTemp, Warning, TEXT("VEHICLE INITIALIZED: %s | Mass: %.0fkg"), *VehicleData->VehicleName.ToString(), VehicleData->Stats.Mass);
    }

    // --- Phase 19-D: Global Symmetry Bind ---
    if (UHeroDrivingSubsystem* DriveSS = GetGameInstance()->GetSubsystem<UHeroDrivingSubsystem>())
    {
        DriveSS->OnSymmetryChanged.AddDynamic(this, &AHeroVehicleBase::HandleSymmetryChange);
        HandleSymmetryChange(DriveSS->bIsLeftHandDrive);
    }
}

void AHeroVehicleBase::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalForce, const FHitResult& Hit)
{
    Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalForce, Hit);

    if (Other && VehicleData)
    {
        // --- Phase 18: Mass-Based Collision (Newton's Law) ---
        float SpeedKmh = 1.0f;
        if (UChaosWheeledVehicleMovementComponent* ChaosMove = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement()))
        {
            SpeedKmh = ChaosMove->GetForwardSpeed() * 0.036f; // cm/s to km/h
        }
        
        float ImpactForce = VehicleData->Stats.Mass * SpeedKmh;

        if (ImpactForce > 5000.f)
        {
             // 1. Calculate Damage for Other (If it has a health component)
             // 2. Apply Physical Impulse to Other based on Mass Ratio
             if (UStaticMeshComponent* OtherMesh = Cast<UStaticMeshComponent>(Other->GetRootComponent()))
             {
                 OtherMesh->AddImpulseAtLocation(HitNormal * -ImpactForce, HitLocation);
             }

             UE_LOG(LogTemp, Log, TEXT("IMPACT: Force %.0f | Speed %.0f"), ImpactForce, SpeedKmh);
        }
    }
}

void AHeroVehicleBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateImpostorLOD();

    if (bIsVoidDriveActive)
    {
        UpdateVoidDrivePhysics();
        HandleSurfaceMagnetism(DeltaTime);
    }

    UpdateVehicleAudio();
    ApplyCameraShake(1.0f); // Auto-scaling internal
}

void AHeroVehicleBase::ToggleVoidDrive()
{
    if (!VehicleData || !VehicleData->Stats.bSupportsAntiGravity) return;

    bIsVoidDriveActive = !bIsVoidDriveActive;

    // 1. Physics Shift (Weightless Mode)
    float TargetMass = bIsVoidDriveActive ? VehicleData->Stats.VoidDriveMass : 1500.f; // 0.1kg vs 1500kg
    GetMesh()->SetMassOverrideInKg(NAME_None, TargetMass, true);

    // 2. VFX & SFX
    if (bIsVoidDriveActive)
    {
        VoidDriveVFX->Activate();
        VoidHumAudio->Activate();
        EngineAudio->SetVolumeMultiplier(0.2f); // Muffle engine
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Purple, TEXT("VOID-DRIVE ACTIVE: HUMANITY GRAVITY ENGAGED!"));
    }
    else
    {
        VoidDriveVFX->Deactivate();
        VoidHumAudio->Deactivate();
        EngineAudio->SetVolumeMultiplier(1.0f);
    }

    // 3. Zoom Out Transition (Camera Logic typically handled in SpringArm / Controller)
}

void AHeroVehicleBase::ApplyTyreDamage(int32 WheelIndex, float Damage)
{
    if (!TyreStatus.IsValidIndex(WheelIndex) || TyreStatus[WheelIndex].bIsBurst) return;

    // Logic for Blowout
    if (Damage > VehicleData->Stats.TyreBurstThreshold)
    {
        TyreStatus[WheelIndex].bIsBurst = true;
        TyreStatus[WheelIndex].bIsRimOnly = true;
        TyreStatus[WheelIndex].FrictionMultiplier = 0.2f; // Extreme slippery

        TriggerTyreBurstFeedback(WheelIndex);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("TYRE BLOWOUT! SHIFTING TO RIM PHYSICS!"));
    }
}

void AHeroVehicleBase::UpdateVehicleAudio()
{
    float Velocity = GetVelocity().Size();
    float SpeedKPH = (Velocity * 0.036f);

    if (bIsVoidDriveActive)
    {
        // Pitch shift based on speed
        float PitchMod = 1.0f + (SpeedKPH / 200.f); 
        VoidHumAudio->SetPitchMultiplier(PitchMod);
    }
    else
    {
        float PitchMod = 0.8f + (SpeedKPH / 150.f);
        EngineAudio->SetPitchMultiplier(PitchMod);
    }
}

void AHeroVehicleBase::ApplyCameraShake(float IntensityMultiplier)
{
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (!PC || !MasterShakeClass) return;

    float Velocity = GetVelocity().Size();
    float SpeedRatio = FMath::Clamp(Velocity / 2000.f, 0.f, 1.f);

    // Scaling intensity: Idle (0.1) -> High-Speed Shiver (1.0)
    float ShadowIntensity = 0.1f + (SpeedRatio * 0.9f);
    
    // In Void-Drive, we dampen high-speed vibration for a "Smooth" hover feel
    if (bIsVoidDriveActive) ShadowIntensity *= 0.3f;

    PC->ClientStartCameraShake(MasterShakeClass, ShadowIntensity * IntensityMultiplier);
}

void AHeroVehicleBase::HandleSurfaceMagnetism(float DeltaTime)
{
    // Simplified: Trace down to find surface and align
    FHitResult Hit;
    FVector Start = GetActorLocation();
    FVector End = Start - (GetActorUpVector() * 500.f);
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
    {
        CurrentSurfaceNormal = Hit.ImpactNormal;
        
        // Match Surface Rotation (Magnetism)
        FRotator TargetRot = UKismetMathLibrary::MakeRotFromZX(CurrentSurfaceNormal, GetActorForwardVector());
        FRotator InterpRot = FMath::RInterpTo(GetActorRotation(), TargetRot, DeltaTime, 5.f);
        SetActorRotation(InterpRot);

        // Hover Force (Suspension override logic)
        // In a full implementation, we'd apply Force to the MoveComp
    }
}

void AHeroVehicleBase::UpdateVoidDrivePhysics()
{
	if (!bIsVoidDriveActive) return;

	UPrimitiveComponent* VehicleMesh = GetMesh();
	if (!VehicleMesh || !VehicleData) return;

	// 1. Mass Manipulation (Weightless Feel)
	VehicleMesh->SetMassScale(NAME_None, VehicleData->Stats.VoidDriveMass);

	// 2. Hover Physics (4-Corner Thrusters)
	TArray<FVector> CornerOffsets;
	FVector BoxExtent = VehicleMesh->Bounds.BoxExtent;
	CornerOffsets.Add(FVector(BoxExtent.X, BoxExtent.Y, -BoxExtent.Z));    // Rear Left
	CornerOffsets.Add(FVector(BoxExtent.X, -BoxExtent.Y, -BoxExtent.Z));   // Rear Right
	CornerOffsets.Add(FVector(-BoxExtent.X, BoxExtent.Y, -BoxExtent.Z));   // Front Left
	CornerOffsets.Add(FVector(-BoxExtent.X, -BoxExtent.Y, -BoxExtent.Z));  // Front Right

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	for (const FVector& Offset : CornerOffsets)
	{
		FVector Start = GetActorTransform().TransformPosition(Offset);
		FVector End = Start - (GetActorUpVector() * 250.f); 
		FHitResult Hit;

		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
		{
			float Distance = Hit.Distance;
			float TargetHover = 100.f; // 1 Meter

			if (Distance < TargetHover)
			{
				float Compression = 1.0f - (Distance / TargetHover);
				FVector HoverForce = GetActorUpVector() * Compression * 1200000.f; 
				
				// Damping for stability
				FVector VelocityAtPoint = VehicleMesh->GetPhysicsLinearVelocityAtPoint(Start);
				float UpVelocity = FVector::DotProduct(VelocityAtPoint, GetActorUpVector());
				FVector DampingForce = GetActorUpVector() * (-UpVelocity * 50000.f);
				
				VehicleMesh->AddForceAtLocation(HoverForce + DampingForce, Start);
			}
		}
	}

	// 3. Humanity Gravity (Radial Force Shockwave)
	float Velocity = GetVelocity().Size();
	float SpeedKPH = (Velocity * 0.036f);
	
	if (SpeedKPH > 15.f)
	{
		float ForceStrength = SpeedKPH * 6000.f;
		float ForceRadius = 400.f + (SpeedKPH * 2.5f);
		
		FCollisionShape Sphere = FCollisionShape::MakeSphere(ForceRadius);
		TArray<FOverlapResult> Overlaps;
		
		if (GetWorld()->OverlapMultiByChannel(Overlaps, GetActorLocation(), FQuat::Identity, ECC_PhysicsBody, Sphere))
		{
			for (const FOverlapResult& Overlap : Overlaps)
			{
				if (UPrimitiveComponent* OtherMesh = Overlap.GetComponent())
				{
					if (OtherMesh->IsSimulatingPhysics())
					{
						OtherMesh->AddRadialImpulse(GetActorLocation(), ForceRadius, ForceStrength, ERadialImpulseFalloff::RIF_Constant, true);
					}
				}
			}
		}
	}
}

void AHeroVehicleBase::TriggerTyreBurstFeedback(int32 WheelIndex)
{
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (PC)
    {
        PC->PlayDynamicForceFeedback(1.0f, 0.5f, true, true, true, true, EDynamicForceFeedbackAction::Start);
        // Directional jolt logic here...
    }
}

void AHeroVehicleBase::HandleSymmetryChange(bool bNewIsLeftHandDrive)
{
    if (!SteeringWheel || !DriverSeat) return;

    // Phase 19-D: Mirror Interior Components
    // LHD: Steering on Left (Negative Y in UE default relative space, usually)
    // RHD: Steering on Right (Positive Y)
    
    float YOffset = 45.f; // 45cm offset from center
    float TargetY = bNewIsLeftHandDrive ? -YOffset : YOffset;

    FVector NewSteeringLoc = SteeringWheel->GetRelativeLocation();
    NewSteeringLoc.Y = TargetY;
    SteeringWheel->SetRelativeLocation(NewSteeringLoc);

    FVector NewSeatLoc = DriverSeat->GetRelativeLocation();
    NewSeatLoc.Y = TargetY;
    DriverSeat->SetRelativeLocation(NewSeatLoc);

    UE_LOG(LogTemp, Log, TEXT("VEHICLE SYMMETRY: Switched to %s-Hand Drive"), bNewIsLeftHandDrive ? TEXT("Left") : TEXT("Right"));
}

void AHeroVehicleBase::UpdateImpostorLOD()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (!PC || !PC->GetPawn()) return;

    float Distance = FVector::Dist(GetActorLocation(), PC->GetPawn()->GetActorLocation());
    float ImpostorThreshold = 5000.f; // 50m

    if (Distance > ImpostorThreshold)
    {
        // SHOW IMPOSTOR, HIDE REAL MESH
        GetMesh()->SetVisibility(false);
        if (ActiveImpostor) ActiveImpostor->SetActorHiddenInGame(false);
    }
    else
    {
        // HIDE IMPOSTOR, SHOW REAL MESH
        GetMesh()->SetVisibility(true);
        if (ActiveImpostor) ActiveImpostor->SetActorHiddenInGame(true);
    }
}

void AHeroVehicleBase::DetectSurfaceNormal()
{
    FHitResult Hit;
    FVector Start = GetActorLocation();
    FVector End = Start - (GetActorUpVector() * 500.f);
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
    {
        CurrentSurfaceNormal = Hit.ImpactNormal;
    }
    else
    {
        CurrentSurfaceNormal = FVector::UpVector;
    }
}
