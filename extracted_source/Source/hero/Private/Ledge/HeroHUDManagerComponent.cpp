// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroHUDManagerComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

UHeroHUDManagerComponent::UHeroHUDManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHeroHUDManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHeroHUDManagerComponent::ToggleVehicleHUD(bool bInVehicle)
{
    APlayerController* PC = Cast<APlayerController>(GetOwner());
    if (!PC || !PC->IsLocalController()) return;

    if (bInVehicle)
    {
        // 1. Create Vehicle HUD if it doesn't exist
        if (!ActiveVehicleHUD && VehicleHUDClass)
        {
            ActiveVehicleHUD = CreateWidget<UUserWidget>(PC, VehicleHUDClass);
        }

        // 2. Add to viewport
        if (ActiveVehicleHUD)
        {
            ActiveVehicleHUD->AddToViewport();
        }
    }
    else
    {
        // 3. Remove from viewport
        if (ActiveVehicleHUD)
        {
            ActiveVehicleHUD->RemoveFromParent();
        }
    }
}
