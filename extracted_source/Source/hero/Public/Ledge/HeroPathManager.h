// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "HeroPathManager.generated.h"

/**
 * AAA URBAN PATH COMPONENT
 * Restricts AI entities to a specific directional track on a spline.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HERO_API UHeroLaneComponent : public USplineComponent
{
	GENERATED_BODY()

public:
	UHeroLaneComponent();

	/** Whether this lane is strictly 'Left' (India) or 'Right' (USA) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic|Path")
	bool bIsLeftHandLane = true;

    /** Current speed limit for this lane (in km/h) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic|Path")
    float SpeedLimit = 40.f;
};

/**
 * AAA MASTER PATH MANAGER
 * Orchestrates all Spline-based directional traffic flow.
 * Handles Intersection Decision Points and Global Symmetry Swap.
 */
UCLASS(Blueprintable, meta = (DisplayName = "Hero Path Manager"))
class HERO_API AHeroPathManager : public AActor
{
	GENERATED_BODY()

public:
	AHeroPathManager();

protected:
	virtual void BeginPlay() override;

public:
	/** Collection of lanes managed by this road actor */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Traffic|Path")
	TArray<UHeroLaneComponent*> Lanes;

    /** Triggered when the Global Symmetry (LHD/RHD) changes */
    UFUNCTION(BlueprintCallable, Category = "Traffic|Path")
    void InvertLanes();

    /** Returns the nearest lane position for an AI entity */
    UFUNCTION(BlueprintCallable, Category = "Traffic|Path")
    FVector GetNearestLanePoint(FVector SourceLocation, bool bPreferLeftHand);

private:
	/** Automatic lane population from child components */
	void DiscoverLanes();
};
