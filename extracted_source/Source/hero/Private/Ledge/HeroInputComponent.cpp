// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroInputComponent.h"
#include "Ledge/HeroInputDataAsset.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"

UHeroInputComponent::UHeroInputComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHeroInputComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHeroInputComponent::NotifyPossessedPawn(APawn* NewPawn)
{
    if (!NewPawn || !MasterInputData) return;

    // Detect if this is a vehicle or character
    EVehicleType PawnType = EVehicleType::HERO_VEHICLE_NONE;

    // Logic: Look for a VehicleType property or interface on the Pawn
    // For now, we manually check if it's a character or vehicle
    if (NewPawn->IsA(ACharacter::StaticClass()))
    {
        PawnType = EVehicleType::HERO_VEHICLE_NONE; // Character
    }
    else
    {
        // Default to Car if it's not a character for testing
        PawnType = EVehicleType::Car;
    }

    //    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("INTERACTION: %s [Reputation: %d]"), *TargetNPC->GetName(), (int32)TargetNPC->NPCFaction));

    // 1. Get correct IMC from DataAsset
    UInputMappingContext* TargetIMC = MasterInputData->GetIMCForPawnType(PawnType);

    if (TargetIMC)
    {
        // 2. Remove old context
        if (LastAppliedIMC)
        {
            RemoveMappingContext(LastAppliedIMC);
        }

        // 3. Apply new context
        ApplyMappingContext(TargetIMC);
        LastAppliedIMC = TargetIMC;
    }
}

void UHeroInputComponent::ApplyMappingContext(UInputMappingContext* Context)
{
    APlayerController* PC = Cast<APlayerController>(GetOwner());
    if (PC && PC->IsLocalController())
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(Context, MasterInputData->MappingPriority);
        }
    }
}

void UHeroInputComponent::RemoveMappingContext(UInputMappingContext* Context)
{
    APlayerController* PC = Cast<APlayerController>(GetOwner());
    if (PC && PC->IsLocalController())
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            Subsystem->RemoveMappingContext(Context);
        }
    }
}
