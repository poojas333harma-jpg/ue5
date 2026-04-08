// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HeroCoreTypes.h"
#include "HeroVehicleData.generated.h"

/**
 * AAA UNIVERSAL VEHICLE DATA
 * Defines performance, physics, and Void-Drive capabilities for a vehicle type.
 */
UCLASS(BlueprintType, meta=(DisplayName="Hero Vehicle Data"))
class HERO_API UHeroVehicleData : public UDataAsset
{
    GENERATED_BODY()

public:
    // --- 1. CLASSIFICATION ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1. Identity")
    FName VehicleName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1. Identity")
    EVehicleType ClassType = EVehicleType::Car;

    // --- 2. PHYSICAL ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="2. Model")
    TObjectPtr<USkeletalMesh> SkeletalMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="2. Model")
    TSubclassOf<class UChaosVehicleWheel> WheelClass;

    // --- 3. PERFORMANCE ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="3. Stats")
    FHeroVehicleStats Stats;

    // --- 4. FEEDBACK & VFX ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="4. Effects")
    class UNiagaraSystem* VoidDriveVFX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="4. Effects")
    class UNiagaraSystem* TyreBurstVFX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="4. Effects")
    class USoundBase* IdleEngineSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="4. Effects")
    class USoundBase* VoidDriveHum;
};
