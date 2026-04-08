// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ledge/HeroCoreTypes.h"
#include "HeroOutpostMaster.generated.h"

/**
 * AAA OUTPOST MASTER
 * Handles the visual and functional rejuvenation of outposts.
 * Swaps between 'Occupied' (Enemy) and 'Captured' (Ally) states with Niagara Poof.
 */
UCLASS()
class HERO_API AHeroOutpostMaster : public AActor
{
	GENERATED_BODY()

public:	
	AHeroOutpostMaster();

protected:
	virtual void BeginPlay() override;

public:
    /** Unique ID for this outpost to sync with the Subsystem */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Outpost")
    FName OutpostID;

    /** Current state to start with */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Outpost")
    EHeroOutpostState CurrentState = EHeroOutpostState::Occupied;

    UFUNCTION(BlueprintCallable, Category = "Outpost")
    void TransitionToState(EHeroOutpostState NewState);

protected:
    /** Actors shown when Occupied (Enemy Fences, Alarms, Spikes) */
    UPROPERTY(EditAnywhere, Category = "Outpost|Visuals")
    TArray<AActor*> OccupiedActors;

    /** Actors shown when Captured (Forge, Library, Flags, Allies) */
    UPROPERTY(EditAnywhere, Category = "Outpost|Visuals")
    TArray<AActor*> CapturedActors;

    /** Niagara Poof/Dust effect for seamless rejuvenation */
    UPROPERTY(EditAnywhere, Category = "Outpost|Visuals")
    class UNiagaraSystem* RejuvenationVFX;

private:
    void UpdateVisuals();
    
    UFUNCTION()
    void OnStateChangedExternal(FName ID, EHeroOutpostState NewState);
};
