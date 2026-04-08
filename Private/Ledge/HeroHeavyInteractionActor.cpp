// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroHeavyInteractionActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

AHeroHeavyInteractionActor::AHeroHeavyInteractionActor()
{
	PrimaryActorTick.bCanEverTick = true;
    
    BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
    RootComponent = BaseMesh;
    
    // Default Cube Mesh (Placeholder)
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/BasicShapes/Cube"));
    if (MeshAsset.Succeeded())
    {
        BaseMesh->SetStaticMesh(MeshAsset.Object);
    }
}

void AHeroHeavyInteractionActor::BeginPlay()
{
	Super::BeginPlay();
}

void AHeroHeavyInteractionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (bIsInteracting)
    {
        UpdateMashing(DeltaTime);
    }
}

void AHeroHeavyInteractionActor::Interact_Implementation(AActor* Interactor)
{
    if (!Interactor) return;

    // 1. Initial Start
    if (!bIsInteracting)
    {
        bIsInteracting = true;
        CurrentInteractor = Interactor;
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("HEAVY FORCE: Mashing to Open Shutter..."));
    }

    // 2. Add Force from Button Press
    MashingData.CurrentProgress += 5.f; // Base force per tap
    
    // 3. Sound Feedback
    UGameplayStatics::PlaySoundAtLocation(this, nullptr, GetActorLocation());
    
    // 4. Success Check
    if (MashingData.CurrentProgress >= MashingData.RequiredForce)
    {
        OnInteractionSuccess();
        bIsInteracting = false;
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("HEAVY FORCE: SUCCESS! Shutter is Open."));
    }
}

void AHeroHeavyInteractionActor::UpdateMashing(float DeltaTime)
{
    // 1. Progress Decay (Stamina-Driven)
    float FinalDecay = MashingData.DecayRate;
    
    // 2. AAA Stamina Penalty Logic
    /*
    if (ACharacter* Char = Cast<ACharacter>(CurrentInteractor))
    {
        // Query Stamina from Heromover or Attribute set
        // float CurrentStamina = Char->GetStamina();
        // if (CurrentStamina < MashingData.MinStaminaThreshold) FinalDecay *= 2.0f; // HARDER
    }
    */

    MashingData.CurrentProgress = FMath::Clamp(MashingData.CurrentProgress - (FinalDecay * DeltaTime), 0.f, MashingData.RequiredForce);
    
    // 3. Broadcast to HUD / Shutter Mesh
    OnProgressUpdated(MashingData.CurrentProgress / MashingData.RequiredForce);
}

void AHeroHeavyInteractionActor::BeginHover_Implementation(AActor* Interactor)
{
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("INTERACT: Use Force to Open?"));
}

void AHeroHeavyInteractionActor::EndHover_Implementation(AActor* Interactor)
{
}
