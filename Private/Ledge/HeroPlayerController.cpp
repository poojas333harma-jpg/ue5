// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroPlayerController.h"
#include "Ledge/HeroWidget_MasterMenu.h"
#include "Ledge/HeroInputComponent.h"
#include "Ledge/HeroCameraManagerComponent.h"
#include "Ledge/HeroHUDManagerComponent.h"
#include "Ledge/HeroDialogueComponent.h"
#include "Ledge/HeroNavigationComponent.h"
#include "Ledge/HeroSenseComponent.h"
#include "Ledge/HeroMultiplayerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"

AHeroPlayerController::AHeroPlayerController()
{
    // Initialize Modular Components
    HeroInput = CreateDefaultSubobject<UHeroInputComponent>(TEXT("HeroInput"));
    HeroCamera = CreateDefaultSubobject<UHeroCameraManagerComponent>(TEXT("HeroCamera"));
    HeroHUD = CreateDefaultSubobject<UHeroHUDManagerComponent>(TEXT("HeroHUD"));
    HeroDialogue = CreateDefaultSubobject<UHeroDialogueComponent>(TEXT("HeroDialogue"));

    HeroNav = CreateDefaultSubobject<UHeroNavigationComponent>(TEXT("HeroNav"));
    HeroSense = CreateDefaultSubobject<UHeroSenseComponent>(TEXT("HeroSense"));
}

void AHeroPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Initially we set Game-Only 
	FInputModeGameOnly GameOnly;
	SetInputMode(GameOnly);
	bShowMouseCursor = false;

    // Initial Input Setup for Character
    if (HeroInput && GetPawn())
    {
        HeroInput->NotifyPossessedPawn(GetPawn());
    }
}

void AHeroPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHeroPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (!InPawn) return;

    // 1. Swap Inputs (IMC_Walking -> IMC_Driving, etc.)
    if (HeroInput)
    {
        HeroInput->NotifyPossessedPawn(InPawn);
    }

    // 2. Adjust Camera Scaling (Mouse vs Train)
    if (HeroCamera)
    {
        HeroCamera->TransitionToPawn(InPawn);
    }

    // 3. Toggle HUD (Show Speedometer/Energy)
    bool bIsVehicle = !InPawn->IsA(ACharacter::StaticClass());
    if (HeroHUD)
    {
        HeroHUD->ToggleVehicleHUD(bIsVehicle);
    }

    UE_LOG(LogTemp, Warning, TEXT("Hero: Possessed %s. All systems synchronized."), *InPawn->GetName());
}

void AHeroPlayerController::OnUnPossess()
{
    // Notify systems of detachment
    if (HeroHUD)
    {
        HeroHUD->ToggleVehicleHUD(false);
    }

    Super::OnUnPossess();
}

void AHeroPlayerController::ToggleMasterMenu()
{
	if (bIsMenuOpen)
	{
		CloseMenu();
	}
	else
	{
		OpenMenu();
	}
}

void AHeroPlayerController::OpenMenu()
{
	if (!MasterMenuClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hero: MasterMenuClass is null in AHeroPlayerController!"));
		return;
	}

	// 1. Create the Widget if it doesn't already exist
	if (!MasterMenuWidget)
	{
		MasterMenuWidget = CreateWidget<UHeroWidget_MasterMenu>(this, MasterMenuClass);
	}

	if (MasterMenuWidget)
	{
		// 2. Add to viewport
		MasterMenuWidget->AddToViewport();

		// 3. Setup Input Mode
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(MasterMenuWidget->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		
		SetInputMode(InputMode);
		bShowMouseCursor = true;

		bIsMenuOpen = true;

		// 4. Optionally pause the game (Uncomment if needed)
		// SetPause(true);
	}
}

void AHeroPlayerController::ToggleHybridMapMode()
{
	// Checklist Phase 14-C: Toggle between Mini-map and Full-screen
	if (HeroNav)
	{
		// Logic to switch UI Widget states
		UE_LOG(LogTemp, Log, TEXT("[HeroPC] Toggling Hybrid Map Mode..."));
	}
}

void AHeroPlayerController::SetNewDestination(FVector NewLocation, EHeroTrailType Type)
{
	if (HeroNav)
	{
		// Inter-map Bridge Logic (Calculates path via Teleporters if needed)
		FVector ActualTarget = HeroNav->GetCalculatedNavPath(NewLocation);
		HeroNav->ActiveGoalLocation = ActualTarget;
		
		UE_LOG(LogTemp, Warning, TEXT("[HeroNav] Target Locked: %s (Inter-map enabled)"), *ActualTarget.ToString());
	}

	CurrentNavData.TargetLocation = NewLocation;
	CurrentNavData.TrailType = Type;
	CurrentNavData.bShowTrail = true;
	
	// Update distance for UI
	if (GetPawn())
	{
		CurrentNavData.DistanceToTarget = FVector::Dist(GetPawn()->GetActorLocation(), NewLocation);
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("NAV TARGET SET! TRAIL GENERATING..."));
}

void AHeroPlayerController::CloseMenu()
{
	if (MasterMenuWidget)
	{
		// 1. Remove from parent
		MasterMenuWidget->RemoveFromParent();
		
		// 2. Restore Game Only Input
		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
		bShowMouseCursor = false;

		bIsMenuOpen = false;

		// 3. Unpause
		// SetPause(false);
	}
}

// --- Phase 60.1 Multiplayer Logic ---

void AHeroPlayerController::Host(FString MapName)
{
	if (UHeroMultiplayerSubsystem* MS = GetGameInstance()->GetSubsystem<UHeroMultiplayerSubsystem>())
	{
		MS->HostGame(FName(*MapName));
	}
}

void AHeroPlayerController::Join(FString IPAddress)
{
	if (UHeroMultiplayerSubsystem* MS = GetGameInstance()->GetSubsystem<UHeroMultiplayerSubsystem>())
	{
		MS->JoinGame(IPAddress);
	}
}
