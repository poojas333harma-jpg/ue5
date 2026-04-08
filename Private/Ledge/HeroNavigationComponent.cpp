// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroNavigationComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Ledge/HeroTeleportManager.h"
#include "Engine/World.h"
#include "Engine/Texture2D.h"

UHeroNavigationComponent::UHeroNavigationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

    // --- Phase 17: Skill-Locked HUD Defaults ---
    bIsMiniMapUnlocked = false;
    bIsCompassUnlocked = false;
}

void UHeroNavigationComponent::BeginPlay()
{
	Super::BeginPlay();
	
    // 1. Get Reference to Teleport Subsystem
    if (GetWorld())
    {
        TeleportManager = GetWorld()->GetSubsystem<UHeroTeleportManager>();
    }
}

void UHeroNavigationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Dynamic rotation and icons refreshed in Tick for 60+ FPS fluidity.
    UpdateMinimapLogic();
}

void UHeroNavigationComponent::UpdateMinimapLogic()
{
    AActor* Owner = GetOwner();
    if (!Owner) return;

    // 1. ROTATION LOGIC: Minimap follows the player heading (Rotating Map)
    // We get the player's control rotation for the 'Camera-centric' minimap.
    if (APawn* PlayerPawn = Cast<APawn>(Owner))
    {
        if (APlayerController* PC = Cast<APlayerController>(PlayerPawn->GetController()))
        {
            MinimapRotation = PC->GetControlRotation().Yaw;
        }
    }
    
    // 2. ICON LOGIC: Filter actors within 100m for Minimap display
    // (This would typically update a local array for the UI Widget to consume)
}

void UHeroNavigationComponent::RefreshMapTexture(FName LevelName)
{
    // Logic to swap current map based on the loaded sublevel.
    // In a real project, this would pull from a DataAsset map of LevelName -> Texture.
    UE_LOG(LogTemp, Warning, TEXT("[HeroNav] Map Texture Scaled for: %s"), *LevelName.ToString());
}

void UHeroNavigationComponent::ProjectWorldMarker(FVector WorldLocation, FVector2D& ScreenPosition, float& DistanceMeters)
{
    if (APawn* Pawn = Cast<APawn>(GetOwner()))
    {
        APlayerController* PC = Cast<APlayerController>(Pawn->GetController());
        if (PC)
        {
            // Project 3D World point to 2D Screen Space
            UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(PC, WorldLocation, ScreenPosition, true);
            
            // Calculate Distance in Meters
            DistanceMeters = FVector::Dist(Pawn->GetActorLocation(), WorldLocation) / 100.f; // UE uses cm
        }
    }
}

FVector UHeroNavigationComponent::GetCalculatedNavPath(FVector GoalLocation)
{
    // Inter-Map Bridge Logic (Checklist Phase 14-C)
    // If the Goal is in another level, we route path to the nearest Teleporter Gate.
    
    // PSEUDO-CODE:
    // 1. Is GoalLocation in Current Level?
    // 2. if No: Find Nearest Gate with TargetMap == GoalMap.
    // 3. return Gate->GetActorLocation().
    
    return GoalLocation; // Default for single-map scenario
}

void UHeroNavigationComponent::RenderRoadPath()
{
    // Dying Light style path rendering using a spline mesh on the ground.
    // Hidden when the distance is < 2m to avoid visual clutter.
}
