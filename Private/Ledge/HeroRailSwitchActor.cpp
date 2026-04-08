// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroRailSwitchActor.h"
#include "Kismet/GameplayStatics.h"

AHeroRailSwitchActor::AHeroRailSwitchActor()
{
	PrimaryActorTick.bCanEverTick = false;
    ActivePathIndex = 0;
}

void AHeroRailSwitchActor::BeginPlay()
{
	Super::BeginPlay();
}

void AHeroRailSwitchActor::Interact_Implementation(AActor* Interactor)
{
    // 1. Cycle through outgoing paths
    if (OutgoingPaths.Num() > 0)
    {
        ActivePathIndex = (ActivePathIndex + 1) % OutgoingPaths.Num();
        
        // 2. Sound Feedback
        UGameplayStatics::PlaySoundAtLocation(this, nullptr, GetActorLocation());
        
        // 3. UI/Cinematic Update (Broadcasting to HUD and Animation)
        OnLeverPulled(ActivePathIndex);
        
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, 
            FString::Printf(TEXT("JUCTION: Rail Switch Toggle to Path [%d]"), ActivePathIndex));
    }
}

void AHeroRailSwitchActor::BeginHover_Implementation(AActor* Interactor)
{
    // In a full implementation, we'd toggle a Highlighting material here.
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("INTERACT: Pull Rail Switch Lever?"));
}

void AHeroRailSwitchActor::EndHover_Implementation(AActor* Interactor)
{
}
