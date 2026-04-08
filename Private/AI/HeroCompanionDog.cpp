// Copyright 2026 Hero Project. All Rights Reserved.

#include "AI/HeroCompanionDog.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AHeroCompanionDog::AHeroCompanionDog()
{
	PrimaryActorTick.bCanEverTick = true;
	FollowDistance = 450.f;
	AnimalProfile = EHeroNPCProfile::Follower;
}

void AHeroCompanionDog::BeginPlay()
{
	Super::BeginPlay();
	
    // Default: Set Samrat (Player Pawn) as leader
    CurrentLeader = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	UE_LOG(LogTemp, Warning, TEXT("DOG COMPANION: Pack Bound to Leader %s"), *CurrentLeader->GetName());
}

void AHeroCompanionDog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentLeader && !bIsLeader)
	{
		ExecuteFormationFollow(DeltaTime);
	}
}

void AHeroCompanionDog::CommandPackAttack(AActor* Target)
{
    if (!Target) return;

    // A. Coordinate Strike: PackMentality
    ExecutePackMentality();
    
    // B. Drive AI into aggressive state
    UCharacterMovementComponent* MoveComp = GetCharacterMovement();
    if (MoveComp) MoveComp->MaxWalkSpeed = 900.f; // Run Speed

    UE_LOG(LogTemp, Warning, TEXT("PACK COMMAND: Attack %s!"), *Target->GetName());
}

void AHeroCompanionDog::CommandPackFormUp()
{
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (MoveComp) MoveComp->MaxWalkSpeed = 450.f; // Walk Speed
	
    UE_LOG(LogTemp, Log, TEXT("PACK COMMAND: Form up on Leader."));
}

void AHeroCompanionDog::ExecuteFormationFollow(float DeltaTime)
{
    if (!CurrentLeader) return;

    float Distance = FVector::Dist(GetActorLocation(), CurrentLeader->GetActorLocation());
    
    if (Distance > FollowDistance)
    {
        // Simple move command (AI behavior would typically use AIController)
        // For this logic, we provide the displacement logic
        FVector Direction = (CurrentLeader->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        AddMovementInput(Direction, 1.0f);
    }
}

void AHeroCompanionDog::UpdateAggroLogic()
{
    // C. Detect nearby threats and bark/guard
}
