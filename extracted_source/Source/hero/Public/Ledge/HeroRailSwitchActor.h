// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ledge/HeroInteractableInterface.h"
#include "HeroRailSwitchActor.generated.h"

class USplineComponent;

/**
 * AAA RAIL SWITCH (THE LEVER)
 * An interactable actor that toggles the 'Active Path' for a rail junction.
 * Samrat can pull the lever to switch between Left/Right/Straight splines.
 */
UCLASS()
class HERO_API AHeroRailSwitchActor : public AActor, public IHeroInteractableInterface
{
	GENERATED_BODY()
	
public:	
	AHeroRailSwitchActor();

protected:
	virtual void BeginPlay() override;

public:	
    // ═════════════════════════════════════════════
    // 1. INTERFACE IMPLEMENTATION
    // ═════════════════════════════════════════════
    virtual void Interact_Implementation(AActor* Interactor) override;
    virtual void BeginHover_Implementation(AActor* Interactor) override;
    virtual void EndHover_Implementation(AActor* Interactor) override;

    // ═════════════════════════════════════════════
    // 2. SWITCH CONFIG
    // ═════════════════════════════════════════════
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HeroRail")
    TArray<TObjectPtr<USplineComponent>> OutgoingPaths;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HeroRail")
    int32 ActivePathIndex = 0;

    /** Hook for the Lever Animation (BP-driven) */
    UFUNCTION(BlueprintImplementableEvent, Category="HeroRail")
    void OnLeverPulled(int32 NewPathIndex);
};
