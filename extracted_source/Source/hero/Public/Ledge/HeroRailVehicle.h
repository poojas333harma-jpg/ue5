// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "HeroInteractableInterface.h"
#include "HeroRailVehicle.generated.h"

class USplineComponent;

/**
 * AAA ADVANCED RAIL VEHICLE
 * Handles Spline-based movement, Bogie coupling, and High-speed Derailment.
 */
UCLASS()
class HERO_API AHeroRailVehicle : public APawn
{
	GENERATED_BODY()

public:
	AHeroRailVehicle();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

    // ═════════════════════════════════════════════
    // 1. RAIL CONFIG
    // ═════════════════════════════════════════════
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HeroRail")
    TObjectPtr<USplineComponent> CurrentTrack;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HeroRail")
    float CurrentDistance = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HeroRail")
    float TargetSpeed = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HeroRail")
    float CurrentSpeed = 0.f;

    // ═════════════════════════════════════════════
    // 2. SURVIVAL DERAILMENT
    // ═════════════════════════════════════════════
    
    UFUNCTION(BlueprintCallable, Category="HeroRail")
    void TriggerDerailment();

    /** Broadcasts when the train crashes so Samrat can transition to 'Injured' state */
    UPROPERTY(BlueprintAssignable, Category="HeroRail")
    FOnInteractionSuccess OnDerailmentCrash;

    // ═════════════════════════════════════════════
    // 3. BOGIE COUPLING
    // ═════════════════════════════════════════════
    
    UFUNCTION(BlueprintCallable, Category="HeroRail")
    void DetachBogie(AHeroRailVehicle* Bogie);

protected:
    bool bIsDerailed = false;
    float RollingFriction = 0.98f; // For decoupled rolling
};
