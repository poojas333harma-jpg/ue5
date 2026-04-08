// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroAnimalBase.h"
#include "MotionWarpingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AHeroAnimalBase::AHeroAnimalBase()
{
	PrimaryActorTick.bCanEverTick = true;

    // Initialize Components
    MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
}

void AHeroAnimalBase::BeginPlay()
{
	Super::BeginPlay();
	
    // 1. Data-Driven Animation loading for Animals
    if (BehaviorDataTable)
    {
        const FName RowName = FName(*UEnum::GetValueAsString(AnimalProfile));
        FAnimalBehaviorSet* FoundSet = BehaviorDataTable->FindRow<FAnimalBehaviorSet>(RowName, TEXT("Animal Behavior Loading"));

        if (FoundSet)
        {
            CurrentBehaviorSet = *FoundSet;
            
            // Sync BlendSpace if needed in AnimInstance
        }
    }
}

void AHeroAnimalBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHeroAnimalBase::PerformVocalize()
{
    if (CurrentBehaviorSet.VocalizeMontage)
    {
        PlayAnimMontage(CurrentBehaviorSet.VocalizeMontage);
    }
}

void AHeroAnimalBase::PerformAction()
{
    if (CurrentBehaviorSet.IdleBreakMontage)
    {
        PlayAnimMontage(CurrentBehaviorSet.IdleBreakMontage);
    }
}

void AHeroAnimalBase::ExecutePackMentality()
{
    // Implementation: Broadcast a signal to nearby animals of the same profile
    // A pack of wolves will all trigger their 'Attack' or 'Howl' montages.
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Animal: PACK MENTALITY TRIGGERED!"));
    
    if (CurrentBehaviorSet.VocalizeMontage)
    {
        PlayAnimMontage(CurrentBehaviorSet.VocalizeMontage);
    }
}

void AHeroAnimalBase::ExecuteFleeReaction(FVector DangerSource)
{
    // Logic: If Horse, RearUp then run away. If Wolf, Snarl then retreat.
    if (CurrentBehaviorSet.ReactionMontage)
    {
        PlayAnimMontage(CurrentBehaviorSet.ReactionMontage);
    }
    
    // Calculate flee direction
    FVector FleeDir = GetActorLocation() - DangerSource;
    FleeDir.Normalize();
    
    // Add movement intent away from danger
    AddMovementInput(FleeDir, 1.0f);
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Animal: FLEEING FROM DANGER!"));
}
