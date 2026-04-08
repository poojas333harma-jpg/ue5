// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroLightSensorComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/DirectionalLight.h"
#include "Components/DirectionalLightComponent.h"
#include "EngineUtils.h"

UHeroLightSensorComponent::UHeroLightSensorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
    VisibilityScalar = 1.0f; // Default to bright (Unsafe)
}

void UHeroLightSensorComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHeroLightSensorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    TimeSinceLastUpdate += DeltaTime;
    if (TimeSinceLastUpdate >= UpdateFrequency)
    {
        CalculateVisibility();
        TimeSinceLastUpdate = 0.0f;
    }
}

void UHeroLightSensorComponent::CalculateVisibility()
{
    AActor* Owner = GetOwner();
    if (!Owner) return;

    UWorld* World = GetWorld();
    FVector Start = Owner->GetActorLocation() + FVector(0,0,90); // Head level probe point

    // 1. SKY VISIBILITY CHECK (Are we indoors?)
    FHitResult SkyHit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Owner);
    
    // Trace straight up
    bool bUnderRoof = World->LineTraceSingleByChannel(SkyHit, Start, Start + FVector(0,0,1000), ECC_Visibility, Params);
    float IndoorDarknessFactor = bUnderRoof ? 0.3f : 1.0f;

    // 2. DIRECTIONAL LIGHT (SUN) CHECK
    float SunVisibility = 0.0f;
    
    ADirectionalLight* PrimaryLight = nullptr;
    for (TActorIterator<ADirectionalLight> It(GetWorld()); It; ++It)
    {
        PrimaryLight = *It;
        break;
    }

    if (PrimaryLight)
    {
        FVector LightDir = PrimaryLight->GetActorForwardVector();
        FHitResult SunHit;
            
        // If the trace towards the sun hits nothing, we are in direct sunlight
        bool bInShadow = World->LineTraceSingleByChannel(SunHit, Start, Start + (LightDir * 5000), ECC_Visibility, Params);
        SunVisibility = bInShadow ? 0.1f : 1.0f;
    }

    // 3. FINAL COMBINATION
    // Base ambient light when indoors is lower. When outdoors, it depends on shadows.
    float TargetScalar = SunVisibility * IndoorDarknessFactor;

    // 4. SMOOTH INTERPOLATION (Prevent flickering)
    VisibilityScalar = FMath::FInterpTo(VisibilityScalar, TargetScalar, UpdateFrequency, 5.0f);

    // DEBUG (Phase 58 Implementation Monitoring)
    // GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, FString::Printf(TEXT("VISIBILITY SCALAR: %.2f"), VisibilityScalar));
}
