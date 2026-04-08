// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroInteractableActorBase.h"

AHeroInteractableActorBase::AHeroInteractableActorBase()
{
	PrimaryActorTick.bCanEverTick = false;

    // 1. Scene Components
    DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
    SetRootComponent(DefaultSceneRoot);

    BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
    BaseMesh->SetupAttachment(RootComponent);
}

void AHeroInteractableActorBase::BeginPlay()
{
	Super::BeginPlay();
}

void AHeroInteractableActorBase::Interact_Implementation(AActor* Interactor)
{
    if (CurrentStatus == ELockStatus::Open)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("Already Open!"));
        return;
    }

    // Check Security Type
    if (SecurityType == EHeroSecurityType::HERO_SECURITY_NONE)
    {
        CurrentStatus = ELockStatus::Open;
        OnSuccessfulUnlock();
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Opened Immediately!"));
    }
    else if (SecurityType == EHeroSecurityType::PhysicalForce)
    {
        TriggerForceEntry();
    }
    else
    {
        // Mini-game logic starts...
        CurrentStatus = ELockStatus::Unlocking;
        if (CircuitHumSFX)
        {
            // Play humming sound for hacking
        }
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("STAGES: Starting Mini-game..."));
    }
}

void AHeroInteractableActorBase::TriggerForceEntry()
{
    // 1. Spawns wood splinters and dust
    /* 
    if (WoodSplinterFX)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WoodSplinterFX, GetActorLocation());
    }
    */
    
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Orange, TEXT("FORCE ENTRY: Wood Splinters & Dust Spawned!"));
    OnFailedAttempt(); // Usually kicks the player back or shakes camera
}

void AHeroInteractableActorBase::BeginHover_Implementation(AActor* Interactor)
{
    // Highlight or Show UI Prompts
    if (BaseMesh)
    {
        BaseMesh->SetRenderCustomDepth(true);
        BaseMesh->SetCustomDepthStencilValue(2); // AAA Highlight Stencil
    }
}

void AHeroInteractableActorBase::EndHover_Implementation(AActor* Interactor)
{
    if (BaseMesh)
    {
        BaseMesh->SetRenderCustomDepth(false);
    }
}

FText AHeroInteractableActorBase::GetInteractionText_Implementation() const
{
    FString DifficultyStr = "";
    switch (Difficulty)
    {
        case EHeroLockDifficulty::Easy:      DifficultyStr = "[Easy]"; break;
        case EHeroLockDifficulty::Medium:    DifficultyStr = "[Medium]"; break;
        case EHeroLockDifficulty::Hard:      DifficultyStr = "[Hard]"; break;
        case EHeroLockDifficulty::Legendary: DifficultyStr = "[Legendary]"; break;
    }

    if (CurrentStatus == ELockStatus::Open) return FText::FromString("Open " + DisplayName.ToString());

    return FText::FromString(FString::Printf(TEXT("Unlock %s %s"), *DisplayName.ToString(), *DifficultyStr));
}
