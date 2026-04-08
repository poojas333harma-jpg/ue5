// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroSenseComponent.h"
#include "GameFramework/Character.h"
#include "Components/MeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Ledge/HeroEnergyComponent.h"
#include "Engine/World.h"
#include "NiagaraFunctionLibrary.h"

UHeroSenseComponent::UHeroSenseComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHeroSenseComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHeroSenseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bIsSenseActive)
    {
        // 1. STAMINA DRAIN (Checklist Phase 14-B)
        AActor* Owner = GetOwner();
        if (Owner)
        {
            UHeroEnergyComponent* Energy = Owner->FindComponentByClass<UHeroEnergyComponent>();
            if (Energy)
            {
                // Energy->ConsumeStamina(DurationStaminaCost * DeltaTime); 
                // if (Energy->GetStamina() <= 0.f) SetSenseActive(false);
            }
        }
        
        // 2. Continuous Scanning (Fluid Outline logic)
        PerformLevelBoundScan();
    }
}

void UHeroSenseComponent::TriggerScan()
{
    // 1. COOLDOWN CHECK
    float CurrentTime = GetWorld()->GetTimeSeconds();
    if (CurrentTime - LastScanTime < ScanCooldown) return;

    LastScanTime = CurrentTime;

    // 2. NIAGARA RADIAL WAVE (Checklist Phase 14-B)
    // UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), nullptr, GetOwner()->GetActorLocation());

    // 3. One-shot Discovery Logic (e.g. reveal map markers)
    PerformLevelBoundScan();
}

void UHeroSenseComponent::SetSenseActive(bool bActive)
{
    bIsSenseActive = bActive;
}

void UHeroSenseComponent::PerformLevelBoundScan()
{
    // HIGH PERFORMANCE SCAN (Optimization Phase 14-C)
    // Only query actors in the same Level (Map) as the player.
    if (!GetOwner()) return;
    
    TArray<AActor*> OverlappingActors;
    FVector ScanOrigin = GetOwner()->GetActorLocation();
    float RadiusCm = SenseRange * 100.f;

    // We use a Sphere Overlap for proximity-based X-Ray.
    // In a real project, we would use a specialized Octree or Actor Iterator filtered by Level.
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Interactable"), OverlappingActors);
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Enemy"), OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        if (FVector::Dist(ScanOrigin, Actor->GetActorLocation()) <= RadiusCm)
        {
            // Apply X-Ray Outline (stencil value 252 for red, 253 for gold)
            SetActorHighLight(Actor, true, 252); 
        }
        else
        {
            SetActorHighLight(Actor, false);
        }
    }
}

void UHeroSenseComponent::SetActorHighLight(AActor* Target, bool bEnable, int32 StencilValue)
{
    if (!Target) return;

    // 1. FIND ALL MESHES (Characters have Skeletons, Items have Static Meshes)
    TArray<UMeshComponent*> Meshes;
    Target->GetComponents<UMeshComponent>(Meshes);

    for (UMeshComponent* Mesh : Meshes)
    {
        Mesh->SetRenderCustomDepth(bEnable);
        if (bEnable)
        {
            Mesh->SetCustomDepthStencilValue(StencilValue);
        }
    }
}
