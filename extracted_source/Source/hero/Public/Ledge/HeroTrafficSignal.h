// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HeroCoreTypes.h"
#include "HeroTrafficSignal.generated.h"

/**
 * AAA URBAN TRAFFIC SIGNAL MASTER
 * Synchronized state machine for Red/Yellow/Green traffic flow.
 * Supports Global Broadcasting to 5,000+ AIs for zero-lag stopping.
 */
UCLASS(Blueprintable, meta = (DisplayName = "Hero Traffic Signal"))
class HERO_API AHeroTrafficSignal : public AActor
{
	GENERATED_BODY()

public:
	AHeroTrafficSignal();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	/** Current Signal State (Green, Yellow, Red) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic|Signal")
	EHeroTrafficState CurrentState = EHeroTrafficState::TS_Red;

	/** Timed durations for each light phase */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic|Signal")
	FHeroTrafficSettings SignalSettings;

    /** Returns the current state as a readable text */
    UFUNCTION(BlueprintCallable, Category = "Traffic|Signal")
    EHeroTrafficState GetSignalState() const { return CurrentState; }

private:
	/** Internal timer to manage light cycles */
	float StateTimer = 0.f;

	/** Transitions between signal states */
	void UpdateSignalCycle(float DeltaTime);

    /** Broadcasts signal state to nearby AI entities */
    void BroadcastSignalState();
};
