// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "HeroCoreTypes.h"
#include "HeroVehicleBase.generated.h"

class UHeroVehicleData;
class UNiagaraComponent;
class UAudioComponent;
class UCameraShakeBase;
class UPhysicsConstraintComponent;
class USplineComponent;

/**
 * AAA MASTER VEHICLE BASE (Universal Framework)
 * Features: Chaos Physics, Void-Drive (Hovering), Tyre Burst, Camera Shake, 
 * Dynamic Audio Pitching, and Surface Magnetism.
 */
UCLASS(Abstract, Blueprintable, meta=(DisplayName="Hero Vehicle Base"))
class HERO_API AHeroVehicleBase : public AWheeledVehiclePawn
{
    GENERATED_BODY()

public:
    AHeroVehicleBase();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalForce, const FHitResult& Hit) override;

public:
    // ═════════════════════════════════════════════
    // 1. DATA & STATE
    // ═════════════════════════════════════════════
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Data")
    TObjectPtr<UHeroVehicleData> VehicleData;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Vehicle|State")
    bool bIsVoidDriveActive = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Vehicle|State")
    TArray<FHeroTyreStatus> TyreStatus;

    // ═════════════════════════════════════════════
    // 2. COMPONENTS
    // ═════════════════════════════════════════════
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Vehicle|VFX")
    TObjectPtr<UNiagaraComponent> VoidDriveVFX;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Vehicle|Audio")
    TObjectPtr<UAudioComponent> EngineAudio;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle|Audio")
    TObjectPtr<UAudioComponent> VoidHumAudio;

    // --- ARTICULATED & SPLINE PHYSICS (Phase 18) ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle|Physics")
    TObjectPtr<UPhysicsConstraintComponent> TrailerConstraint;

    UPROPERTY(BlueprintReadOnly, Category = "Vehicle|Physics")
    TObjectPtr<USplineComponent> ActiveRailSpline;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle|Physics")
    bool bIsAttachedToRail = false;

    // ═════════════════════════════════════════════
    // 3. CORE LOGIC (VOID-DRIVE & TYRE BURST)
    // ═════════════════════════════════════════════
    
    /** Toggles Anti-Gravity mode (Press G) */
    UFUNCTION(BlueprintCallable, Category="Vehicle|Special")
    void ToggleVoidDrive();

    /** Applies damage and triggers blowout VFX/Physics */
    UFUNCTION(BlueprintCallable, Category="Vehicle|Combat")
    void ApplyTyreDamage(int32 WheelIndex, float Damage);

    /** Wall-climbing and Ceiling magnetism */
    UFUNCTION(BlueprintCallable, Category="Vehicle|Physics")
    void HandleSurfaceMagnetism(float DeltaTime);

    // ═════════════════════════════════════════════
    // 4. FEEDBACK & IMMERSION
    // ═════════════════════════════════════════════
    
    /** Dynamically scales camera vibration based on speed and impacts */
    UFUNCTION(BlueprintCallable, Category="Vehicle|Feedback")
    void ApplyCameraShake(float IntensityMultiplier);

    /** Updates engine pitch and Void-Drive hum based on velocity */
    UFUNCTION(BlueprintCallable, Category="Vehicle|Feedback")
    void UpdateVehicleAudio();

    /** Triggers haptic feedback and directional camera jolts */
    UFUNCTION(BlueprintCallable, Category="Vehicle|Feedback")
    void TriggerTyreBurstFeedback(int32 WheelIndex);

protected:
    // --- Internal Helpers ---
    void UpdateVoidDrivePhysics();
    void DetectSurfaceNormal();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle|Feedback")
    TSubclassOf<UCameraShakeBase> MasterShakeClass;

    FVector CurrentSurfaceNormal = FVector::UpVector;

    /** --- Phase 18-C: Impostor & Symmetry --- */
    UFUNCTION()
    void HandleSymmetryChange(bool bNewIsLeftHandDrive);

    /** Spawns and manages the 8-angle sprite impostor at distance */
    void UpdateImpostorLOD();

    UPROPERTY()
    class AHeroVehicleImpostor* ActiveImpostor;

    /** Steering Wheel & Interior Components for Mirroring */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle|Symmetry")
    class UStaticMeshComponent* SteeringWheel;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle|Symmetry")
    class USceneComponent* DriverSeat;
};
