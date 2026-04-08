// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HeroCoreTypes.h"
#include "HeroGrappleComponent.generated.h"

class UCableComponent;

/**
 * AAA GRAPPLING HOOK COMPONENT (Dying Light 2 Inspired)
 * High-fidelity traversal using physics-based cable pulling.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), meta=(DisplayName="Hero Grapple Component"))
class HERO_API UHeroGrappleComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHeroGrappleComponent();

protected:
    virtual void BeginPlay() override;

public:
    // ═════════════════════════════════════════════
    // 1. COMPONENTS & SETTINGS
    // ═════════════════════════════════════════════
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Grapple")
    TObjectPtr<UCableComponent> CableVisual;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grapple|Settings")
    float MaxGrappleDistance = 2500.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grapple|Settings")
    float PullForce = 2000.f;

    // ═════════════════════════════════════════════
    // 2. STATE
    // ═════════════════════════════════════════════
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Grapple")
    bool bIsGrappling = false;

    FVector GrappleTargetPoint;

    // ═════════════════════════════════════════════
    // 3. CORE LOGIC
    // ═════════════════════════════════════════════
    UFUNCTION(BlueprintCallable, Category="Grapple")
    void FireGrapple();

    UFUNCTION(BlueprintCallable, Category="Grapple")
    void StopGrapple();

    /** Call this from Tick or a Timer while grappling */
    UFUNCTION(BlueprintCallable, Category="Grapple")
    void HandleGrappleForce(float DeltaTime);

    /** Line traces for a valid anchor point */
    UFUNCTION(BlueprintPure, Category="Grapple")
    bool FindGrapplePoint(FVector& OutTarget);
};
