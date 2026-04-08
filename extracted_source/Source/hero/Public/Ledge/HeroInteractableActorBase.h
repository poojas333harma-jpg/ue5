// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HeroInteractableInterface.h"
#include "HeroInteractableActorBase.generated.h"

/**
 * AAA INTERACTABLE ACTOR BASE
 * Root class for all world objects (Chests, Doors, Almira) that use 
 * the 4-level security system.
 */
UCLASS(Blueprintable, meta=(DisplayName="Hero Interactable Actor"))
class HERO_API AHeroInteractableActorBase : public AActor, public IHeroInteractableInterface
{
	GENERATED_BODY()
	
public:	
	AHeroInteractableActorBase();

protected:
	virtual void BeginPlay() override;

public:
    // ═════════════════════════════════════════════
    // 1. COMPONENTS
    // ═════════════════════════════════════════════
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    TObjectPtr<USceneComponent> DefaultSceneRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    TObjectPtr<UStaticMeshComponent> BaseMesh;

    // ═════════════════════════════════════════════
    // 2. SECURITY CONFIGURATION
    // ═════════════════════════════════════════════
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Security")
    EHeroSecurityType SecurityType = EHeroSecurityType::Lockpick;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Security")
    EHeroLockDifficulty Difficulty = EHeroLockDifficulty::Easy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Security")
    ELockStatus CurrentStatus = ELockStatus::Locked;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Security")
    FText DisplayName;

    // ═════════════════════════════════════════════
    // 3. CINEMATIC EFFECTS (Niagara & Sound)
    // ═════════════════════════════════════════════
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects|VFX")
    class UNiagaraSystem* WoodSplinterFX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects|VFX")
    class UNiagaraSystem* ElectricSparkFX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects|VFX")
    class UNiagaraSystem* PinSnapFX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects|SFX")
    class USoundBase* MetalSnapSFX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects|SFX")
    class USoundBase* CircuitHumSFX;

    // ═════════════════════════════════════════════
    // 4. INTERFACE IMPLEMENTATION
    // ═════════════════════════════════════════════
    virtual void Interact_Implementation(AActor* Interactor) override;
    virtual void BeginHover_Implementation(AActor* Interactor) override;
    virtual void EndHover_Implementation(AActor* Interactor) override;
    virtual FText GetInteractionText_Implementation() const override;
    virtual EHeroSecurityType GetSecurityType_Implementation() const override { return SecurityType; }
    virtual EHeroLockDifficulty GetLockDifficulty_Implementation() const override { return Difficulty; }

    /** Spawns wood splinters and applies physical impact */
    UFUNCTION(BlueprintCallable, Category="Interaction")
    void TriggerForceEntry();

    // --- Blueprint Hooks ---
    UFUNCTION(BlueprintImplementableEvent, Category="Interaction")
    void OnSuccessfulUnlock();

    UFUNCTION(BlueprintImplementableEvent, Category="Interaction")
    void OnFailedAttempt();
};
