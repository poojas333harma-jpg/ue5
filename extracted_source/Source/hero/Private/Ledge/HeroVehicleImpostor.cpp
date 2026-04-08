// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroVehicleImpostor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

AHeroVehicleImpostor::AHeroVehicleImpostor()
{
	PrimaryActorTick.bCanEverTick = true;

    BillboardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BillboardMesh"));
    SetRootComponent(BillboardMesh);

    WheelSprite_FL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelFL"));
    WheelSprite_FL->SetupAttachment(BillboardMesh);

    WheelSprite_FR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelFR"));
    WheelSprite_FR->SetupAttachment(BillboardMesh);
}

void AHeroVehicleImpostor::BeginPlay()
{
	Super::BeginPlay();
}

void AHeroVehicleImpostor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateViewAngle();
    AnimateWheels(DeltaTime);
}

void AHeroVehicleImpostor::UpdateViewAngle()
{
    APlayerCameraManager* CamManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
    if (!CamManager) return;

    FVector CameraPos = CamManager->GetCameraLocation();
    FVector VehiclePos = GetActorLocation();

    // Calculate rotation to face camera
    FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(VehiclePos, CameraPos);
    RelativeYaw = FRotator::NormalizeAxis(LookAtRot.Yaw - GetActorRotation().Yaw);

    // --- Octagonal Logic (8 Angles) ---
    // Angle Indices: 0=Front, 1=Front-Left, 2=Left, 3=Back-Left, 4=Back, 5=Back-Right, 6=Right, 7=Front-Right
    int32 AngleIndex = FMath::RoundToInt((RelativeYaw + 180.f) / 45.f) % 8;

    // Update Material Parameter (Sprite Index)
    if (BillboardMesh)
    {
        BillboardMesh->SetScalarParameterValueOnMaterials(TEXT("AngleIndex"), (float)AngleIndex);
    }
}

void AHeroVehicleImpostor::AnimateWheels(float DeltaTime)
{
    if (CurrentSpeedKmh > 0.1f)
    {
        float RotationIncrement = (CurrentSpeedKmh * DeltaTime * 100.f); 
        WheelSprite_FL->AddLocalRotation(FRotator(RotationIncrement, 0.f, 0.f));
        WheelSprite_FR->AddLocalRotation(FRotator(RotationIncrement, 0.f, 0.f));
    }
}

// --- DRIVING SUBSYSTEM IMPLEMENTATION ---

#include "Ledge/HeroDrivingSubsystem.h"

void UHeroDrivingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	bIsLeftHandDrive = true; // Default for India
}

void UHeroDrivingSubsystem::SetDrivingSide(bool bInIsLeftHandDrive)
{
    if (bIsLeftHandDrive != bInIsLeftHandDrive)
    {
        bIsLeftHandDrive = bInIsLeftHandDrive;
        OnSymmetryChanged.Broadcast(bIsLeftHandDrive);

        UE_LOG(LogTemp, Warning, TEXT("DRIVING SYMMETRY SWAPPED: %s"), 
            bIsLeftHandDrive ? TEXT("LEFT-HAND (INDIA)") : TEXT("RIGHT-HAND (USA)"));
    }
}
