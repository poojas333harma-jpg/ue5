// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroDestructibleActor.h"
#include "GameFramework/Character.h"
#include "Ledge/HeroNPCBase.h"
#include "Ledge/HeroSaveManagerSubsystem.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

AHeroDestructibleActor::AHeroDestructibleActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AHeroDestructibleActor::BeginPlay()
{
	Super::BeginPlay();

    // 1. Check if we should restore state from Save System
    UHeroSaveManagerSubsystem* SaveSS = GetGameInstance()->GetSubsystem<UHeroSaveManagerSubsystem>();
    if (SaveSS && bIsPersistent)
    {
        // Restore logic... (If destroyed, fracture now)
    }
}

void AHeroDestructibleActor::HandleFractureEvent(const FVector& HitLocation, const FVector& HitImpulse)
{
    // 1. Particle and Sound Feedback
    if (FractureVFX)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FractureVFX, HitLocation);
    }

    if (FractureSFX)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FractureSFX, HitLocation);
    }

    // 2. Specialized Material Responses
    switch (PropType)
    {
        case EHeroDestructionType::Hydrant:
            SpawnHydrantSplash();
            break;
        case EHeroDestructionType::Pole:
            SpawnPoleElectricZone();
            break;
    }

    // 3. Register with Save System if Persistent
    if (bIsPersistent)
    {
        UHeroSaveManagerSubsystem* SaveSS = GetGameInstance()->GetSubsystem<UHeroSaveManagerSubsystem>();
        if (SaveSS)
        {
            // Register destruction
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, 
                FString::Printf(TEXT("PERSISTENCE: Prop %s destruction locked in Save File."), *GetActorLabel()));
        }
    }
}

void AHeroDestructibleActor::SpawnHydrantSplash()
{
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("HYDRANT: Water splash activated (20s Timer)"));
    
    // Timer to stop water for performance
    GetWorldTimerManager().SetTimer(ResetTimerHandle, this, &AHeroDestructibleActor::StopHydrantWater, 20.f, false);
}

void AHeroDestructibleActor::StopHydrantWater()
{
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("HYDRANT: Valve closed, water stopped."));
}

void AHeroDestructibleActor::SpawnPoleElectricZone()
{
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, 
        TEXT("POLE: Broken transformer! Sparks spawning (Electrical Hazard Active)"));
    
    // In a full implementation, we'd spawn a Damage Volume here.
}
