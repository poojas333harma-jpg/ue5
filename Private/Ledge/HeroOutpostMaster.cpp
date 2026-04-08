// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroOutpostMaster.h"
#include "NiagaraFunctionLibrary.h"
#include "Ledge/HeroOutpostSubsystem.h"

AHeroOutpostMaster::AHeroOutpostMaster()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AHeroOutpostMaster::BeginPlay()
{
	Super::BeginPlay();

    // Subscribe to the global outpost subsystem to sync logic
    if (UHeroOutpostSubsystem* Subsystem = GetWorld()->GetSubsystem<UHeroOutpostSubsystem>())
    {
        Subsystem->OnOutpostStateChanged.AddDynamic(this, &AHeroOutpostMaster::OnStateChangedExternal);
        
        // Initial state sync
        CurrentState = Subsystem->GetOutpostState(OutpostID);
        UpdateVisuals();
    }
}

void AHeroOutpostMaster::TransitionToState(EHeroOutpostState NewState)
{
    if (CurrentState == NewState) return;

    CurrentState = NewState;

    // 1. Trigger Niagara "Poof" Effect
    if (RejuvenationVFX)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), RejuvenationVFX, GetActorLocation());
    }

    // 2. Perform seamless actor swap
    UpdateVisuals();
    
    // 3. Sync with global subsystem
    if (UHeroOutpostSubsystem* Subsystem = GetWorld()->GetSubsystem<UHeroOutpostSubsystem>())
    {
        Subsystem->SetOutpostState(OutpostID, NewState);
    }
}

void AHeroOutpostMaster::UpdateVisuals()
{
    bool bIsCaptured = (CurrentState == EHeroOutpostState::Captured || CurrentState == EHeroOutpostState::Upgraded);

    // Toggle enemy actors
    for (AActor* Actor : OccupiedActors)
    {
        if (Actor)
        {
            Actor->SetActorHiddenInGame(bIsCaptured);
            Actor->SetActorEnableCollision(!bIsCaptured);
        }
    }

    // Toggle ally actors
    for (AActor* Actor : CapturedActors)
    {
        if (Actor)
        {
            Actor->SetActorHiddenInGame(!bIsCaptured);
            Actor->SetActorEnableCollision(bIsCaptured);
        }
    }
}

void AHeroOutpostMaster::OnStateChangedExternal(FName ID, EHeroOutpostState NewState)
{
    if (ID == OutpostID)
    {
        TransitionToState(NewState);
    }
}
