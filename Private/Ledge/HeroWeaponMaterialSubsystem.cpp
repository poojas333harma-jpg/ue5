// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroWeaponMaterialSubsystem.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/MeshComponent.h"
#include "Ledge/HeroEnvironmentSubsystem.h"
#include "TimerManager.h"

void UHeroWeaponMaterialSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
    
    // Check for environment sync every 2 seconds
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_WearUpdate, this, &UHeroWeaponMaterialSubsystem::PeriodicWearCheck, 2.0f, true);
}

void UHeroWeaponMaterialSubsystem::UpdateWeaponMaterialWear(UMeshComponent* Mesh, float DurabilityRatio)
{
    if (!Mesh) return;

    if (!CurrentWeaponDMI || CurrentWeaponDMI->Parent != Mesh->GetMaterial(0))
    {
        CurrentWeaponDMI = Mesh->CreateDynamicMaterialInstance(0);
    }

    if (CurrentWeaponDMI)
    {
        // RUST LOGIC: At <30% durability, rust starts (Ratio < 0.3)
        float RustAmount = FMath::Clamp((0.3f - DurabilityRatio) / 0.3f, 0.0f, 1.0f);
        CurrentWeaponDMI->SetScalarParameterValue(FName("Rust_Amount"), RustAmount);

        // SCRATCH LOGIC: Scratches exist even if rust is cleaned
        float ScratchAmount = FMath::Clamp(1.0f - DurabilityRatio, 0.0f, 1.0f);
        CurrentWeaponDMI->SetScalarParameterValue(FName("Scratch_Amount"), ScratchAmount);
        
        // GORE LOGIC: Blood splatter (Placeholder)
        // CurrentWeaponDMI->SetScalarParameterValue(FName("Blood_Alpha"), 0.2f);
    }
}

void UHeroWeaponMaterialSubsystem::PeriodicWearCheck()
{
    // Environment Sync: If raining, increase Wetness and accelerate Rust logic implicitly in next update
    UHeroEnvironmentSubsystem* Env = GetWorld()->GetSubsystem<UHeroEnvironmentSubsystem>();
    if (Env && CurrentWeaponDMI)
    {
        bool bIsRaining = (Env->GetCurrentWeather() == EHeroWeatherType::Rain || Env->GetCurrentWeather() == EHeroWeatherType::Storm);
        CurrentWeaponDMI->SetScalarParameterValue(FName("Wetness_Intensity"), bIsRaining ? 0.8f : 0.0f);
    }
}
