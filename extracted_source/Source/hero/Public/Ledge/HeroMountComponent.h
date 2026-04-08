// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HeroMountComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HERO_API UHeroMountComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHeroMountComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    /** ═════════════════════════════════════════════
     * 1. QUAD-LEG IK DATA
     * ═════════════════════════════════════════════ */

    UPROPERTY(BlueprintReadOnly, Category = "Mount|IK")
    float FootOffset_FL;

    UPROPERTY(BlueprintReadOnly, Category = "Mount|IK")
    float FootOffset_FR;

    UPROPERTY(BlueprintReadOnly, Category = "Mount|IK")
    float FootOffset_BL;

    UPROPERTY(BlueprintReadOnly, Category = "Mount|IK")
    float FootOffset_BR;

    UPROPERTY(EditAnywhere, Category = "Mount|IK")
    float TraceDistance = 100.f;

    UPROPERTY(EditAnywhere, Category = "Mount|IK")
    float IKInterpSpeed = 15.f;

    /** ═════════════════════════════════════════════
     * 2. AI FEAR INSTINCTS
     * ═════════════════════════════════════════════ */

    UPROPERTY(EditAnywhere, Category = "Mount|Fear")
    float FearRadius = 500.f; // 5 Meters

    UPROPERTY(EditAnywhere, Category = "Mount|Fear")
    class UAnimMontage* RearUpMontage;

private:
    void UpdateLegIK(float DeltaTime);
    void UpdateFearInstincts();
    
    float CalculateFootOffset(FName SocketName);

    bool bIsFearing = false;
};
