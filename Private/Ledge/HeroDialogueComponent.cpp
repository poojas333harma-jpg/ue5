// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroDialogueComponent.h"
#include "Ledge/HeroNPCBase.h"
#include "Ledge/HeroInteractionWidget.h"
#include "Ledge/HeroLanguageManager.h"
#include "Ledge/HeroOutpostSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "MediaPlateComponent.h"

UHeroDialogueComponent::UHeroDialogueComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bInInteraction = false;
    
    // Default Karma Values
    KarmaValue = 0.0f;
    ReputationTier = EHeroReputationTier::Neutral;
}

void UHeroDialogueComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHeroDialogueComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bInInteraction)
    {
        UpdateCinematicCamera(DeltaTime);
    }
}

void UHeroDialogueComponent::StartInteraction(AHeroNPCBase* TargetNPC)
{
    if (!TargetNPC || !InteractionDataTable || bInInteraction) return;

    // 1. OTS Camera Blend (3s Smooth)
    APlayerController* PC = Cast<APlayerController>(GetOwner());
    if (PC && PC->GetPawn())
    {
        // Smoothly blend to an over-the-shoulder frame
        PC->SetViewTargetWithBlend(TargetNPC, 3.0f, EViewTargetBlendFunction::VTBlend_Cubic);
    }

    // 2. Emotion Swap (3D -> 4K Alpha Video)
    if (UMediaPlateComponent* MediaPlate = TargetNPC->FindComponentByClass<UMediaPlateComponent>())
    {
        TargetNPC->GetMesh()->SetHiddenInGame(true);
        // Play "Smile.mp4" as default initial state
        MediaPlate->SetActive(true);
    }

    // 3. Fetch Interaction Logic from DataTable
    FHeroNPCInteractionRow* Row = InteractionDataTable->FindRow<FHeroNPCInteractionRow>(FName(*UEnum::GetValueAsString(TargetNPC->NPCProfile)), TEXT("Finding NPC Options"));

    if (Row && InteractionMenuClass)
    {
        // 3. Spawn Menu Widget
        ActiveWidget = CreateWidget<UHeroInteractionWidget>(PC, InteractionMenuClass);
        if (ActiveWidget)
        {
            ActiveWidget->InitializeInteraction(TargetNPC, *Row);
            ActiveWidget->AddToViewport();
        }

        // 4. Multi-language Audio Sync
        // Auto-switches based on the Global Language ID
        if (UHeroLanguageManager* LangManager = GetWorld()->GetGameInstance()->GetSubsystem<UHeroLanguageManager>())
        {
            // logic to trigger greeting audio line
            // LangManager->PlayLocalizedLine(TargetNPC, FName("GREETING"));
        }
    }

    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("CINEMATIC INTERACTION STARTED..."));
}

void UHeroDialogueComponent::EndInteraction()
{
    bInInteraction = false;
    CurrentNPC = nullptr;

    if (ActiveWidget)
    {
        ActiveWidget->RemoveFromParent();
    }

    // Restore Camera
    APlayerController* PC = Cast<APlayerController>(GetOwner());
    if (PC && PC->GetPawn())
    {
        if (UCameraComponent* CameraComp = PC->GetPawn()->FindComponentByClass<UCameraComponent>())
        {
            CameraComp->SetFieldOfView(DefaultFieldOfView);
        }
    }
}

void UHeroDialogueComponent::ProcessChoiceKarma(float KarmaChange)
{
    KarmaValue = FMath::Clamp(KarmaValue + KarmaChange, -1.0f, 1.0f);

    // Reputation Sync Thresholds
    if (KarmaValue > 0.5f)
    {
        ReputationTier = EHeroReputationTier::Respected;
    }
    else if (KarmaValue < -0.5f)
    {
        ReputationTier = EHeroReputationTier::Notorious;
    }
    else
    {
        ReputationTier = EHeroReputationTier::Neutral;
    }

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("KARMA UPDATED: %.2f [Tier: %s]"), KarmaValue, *UEnum::GetValueAsString(ReputationTier)));
}

void UHeroDialogueComponent::UpdateCinematicCamera(float DeltaTime)
{
    // Blend handled by SetViewTargetWithBlend, but we can add secondary micro-adjustments here if needed
}
