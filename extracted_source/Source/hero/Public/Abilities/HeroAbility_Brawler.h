// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Ledge/HeroCoreTypes.h"
#include "HeroAbility_Brawler.generated.h"

class UAnimMontage;
class UCameraShakeBase;

/**
 * PHASE 23-B: BRUTAL VIGILANTE - BRAWLER ABILITY
 * Provides MMA close-quarters combat for Samrat.
 * Features: Light/Heavy combos, Hit-Stop, and Screen-Shake on impact.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HERO_API UHeroAbility_Brawler : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHeroAbility_Brawler();

protected:
	virtual void BeginPlay() override;

public:	
	// ═════════════════════════════════════════════
	// 1. COMBAT ACTIONS
	// ═════════════════════════════════════════════

	UFUNCTION(BlueprintCallable, Category = "Hero|Combat")
	void ExecuteLightAttack();

	UFUNCTION(BlueprintCallable, Category = "Hero|Combat")
	void ExecuteHeavyAttack();

    /** MMA Takedowns and Finisher logic */
    UFUNCTION(BlueprintCallable, Category = "Hero|Combat")
    void ExecuteDogaFinisher(AActor* TargetNPC);

	// ═════════════════════════════════════════════
	// 2. IMPACT EFFECTS (Hit-Stop & Screen-Shake)
	// ═════════════════════════════════════════════

    /** Applies a momentary time-dilation pause to emphasize impact */
    void ApplyHitStop(float Duration = 0.05f);

    /** Triggers camera vibration for powerful blows */
    void TriggerImpactShake(float Intensity = 1.0f);

    UPROPERTY(EditDefaultsOnly, Category = "Hero|Combat|VFX")
    TSubclassOf<UCameraShakeBase> HeavyImpactShakeClass;

private:
	int32 ComboCounter = 0;
    float LastAttackTime = 0.f;
	
    UPROPERTY()
    class AHeroCharacter* OwnerHero;

    void ResetCombo();
};
