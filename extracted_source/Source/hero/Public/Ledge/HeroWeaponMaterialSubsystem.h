// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Ledge/HeroCoreTypes.h"
#include "HeroWeaponMaterialSubsystem.generated.h"

/**
 * MASTER WEAPON MATERIAL SUBSYSTEM
 * Handles dynamic shaders for 'Rust', 'Scratches', and 'Wetness' based on durability and weather.
 */
UCLASS()
class HERO_API UHeroWeaponMaterialSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    /** 
     * Applies dynamic wear parameters to the mesh.
     * @param Mesh - Weapon Mesh.
     * @param DurabilityRatio - 0.0 to 1.0.
     */
    UFUNCTION(BlueprintCallable, Category = "Hero|Visuals")
    void UpdateWeaponMaterialWear(UMeshComponent* Mesh, float DurabilityRatio);

private:
    /** Cached DMI to prevent constant allocation */
    UPROPERTY()
    class UMaterialInstanceDynamic* CurrentWeaponDMI;

    FTimerHandle TimerHandle_WearUpdate;
    void PeriodicWearCheck();
};
