// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HeroCameraManagerComponent.generated.h"

class USpringArmComponent;

/**
 * MODULAR CAMERA MANAGER
 * Automatically adjusts the spring arm based on pawn size (Mouse vs Train).
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HERO_API UHeroCameraManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHeroCameraManagerComponent();

    /** 
     * Lerps the camera to the new pawn's dimensions. 
     * @param NewPawn - The possessed actor.
     */
    UFUNCTION(BlueprintCallable, Category = "Hero|Camera")
    void TransitionToPawn(APawn* NewPawn);

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	USpringArmComponent* ActiveSpringArm;

    float TargetArmLength = 300.f;
    float CurrentArmLength = 300.f;
    float TransitionSpeed = 5.0f;

    void AdjustSpringArm(APawn* NewPawn);
};
