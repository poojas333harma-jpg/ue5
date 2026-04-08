// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ledge/HeroInteractableInterface.h"
#include "HeroCoreTypes.h"
#include "HeroHeavyInteractionActor.generated.h"

/**
 * AAA HEAVY INTERACTION (THE SHUTTER / PIPE)
 * Provides the stamina-linked button mashing logic for heavy props.
 * Features: Success/Fail delegates, Stamina scaling, and Progress broadcasting.
 */
UCLASS()
class HERO_API AHeroHeavyInteractionActor : public AActor, public IHeroInteractableInterface
{
	GENERATED_BODY()
	
public:	
	AHeroHeavyInteractionActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

    // ═════════════════════════════════════════════
    // 1. INTERFACE IMPLEMENTATION
    // ═════════════════════════════════════════════
    virtual void Interact_Implementation(AActor* Interactor) override;
    virtual void BeginHover_Implementation(AActor* Interactor) override;
    virtual void EndHover_Implementation(AActor* Interactor) override;

    // ═════════════════════════════════════════════
    // 2. INTERACTION CONFIG
    // ═════════════════════════════════════════════
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HeroInteraction")
    FHeroMashingData MashingData;

    UPROPERTY(BlueprintReadOnly, Category="HeroInteraction")
    bool bIsInteracting = false;

    UPROPERTY(BlueprintReadOnly, Category="HeroInteraction")
    AActor* CurrentInteractor;

    // ═════════════════════════════════════════════
    // 3. BLUEPRINT HOOKS (ANIMATION)
    // ═════════════════════════════════════════════
    
    /** Hook for the Shutter/Pipe Animation (Scale with Progress) */
    UFUNCTION(BlueprintImplementableEvent, Category="HeroInteraction")
    void OnProgressUpdated(float ProgressFrac);

    UFUNCTION(BlueprintImplementableEvent, Category="HeroInteraction")
    void OnInteractionSuccess();

protected:
    void UpdateMashing(float DeltaTime);
    
    UPROPERTY(VisibleAnywhere)
    class UStaticMeshComponent* BaseMesh; // The "Cube" Placeholder
};
