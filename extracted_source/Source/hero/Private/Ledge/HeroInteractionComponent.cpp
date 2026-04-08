#include "Ledge/HeroInteractionComponent.h"
#include "GameFramework/Character.h"
#include "Ledge/HeroNPCBase.h"
#include "Ledge/HeroInteractableInterface.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

UHeroInteractionComponent::UHeroInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHeroInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHeroInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Continuous tracing for UI prompts (Open Box, Interact Door)
    TraceForInteractables();
}

void UHeroInteractionComponent::TraceForInteractables()
{
    APawn* OwnerPawn = Cast<APawn>(GetOwner());
    if (!OwnerPawn) return;

    FVector Start = OwnerPawn->GetActorLocation() + FVector(0,0,60); // Eye level
    FVector End = Start + (OwnerPawn->GetActorForwardVector() * InteractionRange);
    
    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(OwnerPawn);

    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
    {
        AActor* HitActor = Hit.GetActor();
        if (HitActor && HitActor->GetClass()->ImplementsInterface(UHeroInteractableInterface::StaticClass()))
        {
            if (HitActor != LastInteractedActor)
            {
                // Hover Effects
                if (LastInteractedActor)
                {
                    IHeroInteractableInterface::Execute_EndHover(LastInteractedActor, OwnerPawn);
                }
                CurrentInteractable = HitActor;
                IHeroInteractableInterface::Execute_BeginHover(HitActor, OwnerPawn);
                LastInteractedActor = HitActor;
            }
            return;
        }
    }

    // No hit or not interactable
    if (LastInteractedActor)
    {
        IHeroInteractableInterface::Execute_EndHover(LastInteractedActor, OwnerPawn);
        CurrentInteractable = nullptr;
        LastInteractedActor = nullptr;
    }
}

void UHeroInteractionComponent::StartLockpick(EHeroLockDifficulty Difficulty)
{
    CurrentDifficulty = Difficulty;
    CurrentPinDurability = 100.f;

    // Randomize sweet spot between -90 and 90 degrees
    TargetSweetSpotAngle = FMath::FRandRange(-90.f, 90.f);

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, 
        FString::Printf(TEXT("LOCKPICK STARTED: Difficulty=%d"), (int32)Difficulty));
}

void UHeroInteractionComponent::UpdateLockpick(float PinAngle, float BarrelRotation, float DeltaTime, bool& bSuccess, bool& bPinSnapped)
{
    bSuccess = false;
    bPinSnapped = false;

    float SweetSpot = GetSweetSpotSize();
    float Diff = FMath::Abs(PinAngle - TargetSweetSpotAngle);

    // 1. Check if Pin is in the Sweet Spot
    if (Diff <= SweetSpot)
    {
        // Barrel can rotate more as we get closer to center of sweet spot
        float MaxPossibleRotation = 90.f * (1.0f - (Diff / SweetSpot));
        
        if (BarrelRotation >= 85.f && MaxPossibleRotation >= 85.f)
        {
            bSuccess = true;
            OnInteractionSuccess.Broadcast(GetOwner(), CurrentInteractable);
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("LOCK OPENED - BROADCASTING SUCCESS!"));
        }
    }
    else
    {
        // Barrel is blocked by pin!
        if (BarrelRotation > 5.f)
        {
            // Vibrate and damage pin
            float DamageRate = 25.f * (1.0f + static_cast<float>(CurrentDifficulty));
            CurrentPinDurability -= DamageRate * DeltaTime;

            if (CurrentPinDurability <= 0.f)
            {
                bPinSnapped = true;
                GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("PIN SNAPPED! (Niagara Sparks Spawned)"));
            }
        }
    }
}

void UHeroInteractionComponent::StartElectronicHack(EHeroLockDifficulty Difficulty)
{
    CurrentDifficulty = Difficulty;
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("CIRCUIT HACK STARTED: Connect the Nodes!"));
}

void UHeroInteractionComponent::UpdateHackPuzzle(int32 NodeIndex, bool& bLevelComplete)
{
    // Simplified Logic: In a full game, this would verify circuit flow
    // For now, assume NodeIndex represents progress
    bLevelComplete = (NodeIndex >= 5); 

    if (bLevelComplete)
    {
        OnInteractionSuccess.Broadcast(GetOwner(), CurrentInteractable);
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("SYSTEM OVERRIDDEN - ACCESS GRANTED!"));
    }
}

void UHeroInteractionComponent::StartSearchNPC(AHeroNPCBase* TargetNPC)
{
    if (!TargetNPC || !TargetNPC->bIsDead || TargetNPC->bIsSearched) return;

    // Logic: In BP, you would play a "Tatolna" (Search) montage here.
    // Once the montage finishes, call TargetNPC->MarkAsSearched().

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("INTERACTION: %s [Reputation: %d]"), *TargetNPC->GetName(), (int32)TargetNPC->NPCFaction));
    OnInteractionSuccess.Broadcast(GetOwner(), TargetNPC);
}

float UHeroInteractionComponent::GetSweetSpotSize() const
{
    switch (CurrentDifficulty)
    {
        case EHeroLockDifficulty::Easy:      return 45.f;
        case EHeroLockDifficulty::Medium:    return 25.f;
        case EHeroLockDifficulty::Hard:      return 10.f;
        case EHeroLockDifficulty::Legendary: return 3.f;
        default: return 20.f;
    }
}
