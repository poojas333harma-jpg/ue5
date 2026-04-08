// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Ledge/HeroCoreTypes.h"
#include "HeroDialogueComponent.generated.h"

class AHeroNPCBase;
class UHeroInteractionWidget;

/**
 * MASTER DIALOGUE SYSTEM
 * Handles the logic for interacting with NPCs, Cinematic Camera Zoom, and UI spawning.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HERO_API UHeroDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHeroDialogueComponent();

    /** 
     * Starts interaction with a target NPC. 
     * @param TargetNPC - The actor to interact with.
     */
    UFUNCTION(BlueprintCallable, Category = "Hero|Dialogue")
    void StartInteraction(AHeroNPCBase* TargetNPC);

    /** Ends interaction and restores the camera. */
    UFUNCTION(BlueprintCallable, Category = "Hero|Dialogue")
    void EndInteraction();

    UFUNCTION(BlueprintCallable, Category = "Hero|Dialogue")
    void ProcessChoiceKarma(float KarmaChange);

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(EditDefaultsOnly, Category = "Hero|Dialogue")
    UDataTable* InteractionDataTable;

    UPROPERTY(EditDefaultsOnly, Category = "Hero|Dialogue")
    TSubclassOf<UHeroInteractionWidget> InteractionMenuClass;

    UPROPERTY(EditDefaultsOnly, Category = "Hero|Camera")
    float ZoomSpeed = 5.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Hero|Camera")
    float FocusFieldOfView = 45.0f;

    /** ═════════════════════════════════════════════
     * KARMA & REPUTATION (PHASE 15)
     * ═════════════════════════════════════════════ */
    UPROPERTY(BlueprintReadOnly, Category = "Hero|Dialogue")
    float KarmaValue;

    UPROPERTY(BlueprintReadOnly, Category = "Hero|Dialogue")
    EHeroReputationTier ReputationTier;

private:
    UPROPERTY()
    AHeroNPCBase* CurrentNPC;

    UPROPERTY()
    UHeroInteractionWidget* ActiveWidget;

    float DefaultFieldOfView = 90.0f;
    bool bInInteraction = false;

    /** Internal logic for Cinematic Zoom */
    void UpdateCinematicCamera(float DeltaTime);
};
