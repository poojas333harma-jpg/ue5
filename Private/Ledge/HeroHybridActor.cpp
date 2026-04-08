// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroHybridActor.h"
#include "MediaPlateComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AHeroHybridActor::AHeroHybridActor()
{
	PrimaryActorTick.bCanEverTick = true;

    // 0. Default Root
    DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
    RootComponent = DefaultRoot;

    // 1. Media Plate for Video Sprites (Non-visual controller)
    MediaPlate = CreateDefaultSubobject<UMediaPlateComponent>(TEXT("MediaPlate"));

    // 2. Visual Mesh for Video Sprite (Plane)
    VideoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VideoMesh"));
    VideoMesh->SetupAttachment(RootComponent);
    VideoMesh->SetHiddenInGame(false);

    // 3. 3D Skeletal Mesh Proxy
    Mesh3D = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh3D"));
    Mesh3D->SetupAttachment(RootComponent);
    Mesh3D->SetRelativeLocation(FVector(0, 0, -90)); // Adjust for character height
    Mesh3D->SetHiddenInGame(true);

    // 4. Decal Shadow (Soft Gradient)
    FootShadow = CreateDefaultSubobject<UDecalComponent>(TEXT("FootShadow"));
    FootShadow->SetupAttachment(RootComponent);
    FootShadow->SetRelativeRotation(FRotator(90, 0, 0));
    FootShadow->DecalSize = FVector(64, 64, 64);
}

void AHeroHybridActor::BeginPlay()
{
	Super::BeginPlay();
	UpdateHybridState();
}

void AHeroHybridActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    UpdateHybridState();

    if (bIsMediaActive && !bIsCloseRange)
    {
        UpdateMediaPerspective();
    }
}

void AHeroHybridActor::UpdateHybridState()
{
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!PlayerPawn) return;

    float Distance = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());

    // 1. Check for Close Range (3D Swap)
    bool bTargetCloseRange = Distance <= SwapDistance;
    if (bTargetCloseRange != bIsCloseRange)
    {
        bIsCloseRange = bTargetCloseRange;
        Mesh3D->SetHiddenInGame(!bIsCloseRange);
        VideoMesh->SetHiddenInGame(bIsCloseRange);

        if (bIsCloseRange)
        {
            MediaPlate->Close();
        }
    }

    // 2. Media Pooling (Active Media logic)
    bool bTargetMediaActive = Distance <= ActiveMediaRadius;
    if (bTargetMediaActive != bIsMediaActive)
    {
        bIsMediaActive = bTargetMediaActive;
        // Logic: When too far, we can stop the media player to save performance
        if (!bIsMediaActive)
        {
            MediaPlate->Close();
        }
    }
}

void AHeroHybridActor::UpdateMediaPerspective()
{
    APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
    if (!CameraManager) return;

    FVector CameraLocation = CameraManager->GetCameraLocation();
    FVector DirectionToCamera = (CameraLocation - GetActorLocation()).GetSafeNormal();
    FVector ActorForward = GetActorForwardVector();
    FVector ActorRight = GetActorRightVector();

    float DotFront = FVector::DotProduct(ActorForward, DirectionToCamera);
    float DotRight = FVector::DotProduct(ActorRight, DirectionToCamera);

    UMediaSource* TargetSource = Video_Front;

    // Perspective Logic (Dot Product based)
    if (DotFront > 0.5f)
    {
        TargetSource = Video_Front;
    }
    else if (DotRight > 0.3f)
    {
        TargetSource = Video_Right;
    }
    else if (DotRight < -0.3f)
    {
        TargetSource = Video_Left;
    }

    // Update Media if it changed
    // Note: In a real pooling system, we would handle the OpenSource only when necessary.
}
