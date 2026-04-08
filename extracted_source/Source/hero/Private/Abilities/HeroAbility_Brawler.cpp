// Copyright 2026 Hero Project. All Rights Reserved.

#include "Abilities/HeroAbility_Brawler.h"
#include "Ledge/HeroCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraShakeBase.h"
#include "TimerManager.h"

UHeroAbility_Brawler::UHeroAbility_Brawler()
{
	PrimaryComponentTick.bCanEverTick = true;
	ComboCounter = 0;
}

void UHeroAbility_Brawler::BeginPlay()
{
	Super::BeginPlay();
	OwnerHero = Cast<AHeroCharacter>(GetOwner());
}

void UHeroAbility_Brawler::ExecuteLightAttack()
{
    float CurrentTime = GetWorld()->GetTimeSeconds();
    if (CurrentTime - LastAttackTime > 1.5f) ResetCombo();

    ComboCounter++;
    LastAttackTime = CurrentTime;

    // A. Trigger Montage based on Combo Index
    // (Actual animation playback logic typically through OwnerHero->PlayAnimMontage)
    
    // B. Impact Effect on Success
    ApplyHitStop(0.04f);
    TriggerImpactShake(0.3f);
    
    UE_LOG(LogTemp, Warning, TEXT("BRAWLER: Light Strike %d"), ComboCounter);
}

void UHeroAbility_Brawler::ExecuteHeavyAttack()
{
    ApplyHitStop(0.08f);
    TriggerImpactShake(1.2f);
    
    ResetCombo();
    UE_LOG(LogTemp, Warning, TEXT("BRAWLER: Heavy Brutal Blow!"));
}

void UHeroAbility_Brawler::ExecuteDogaFinisher(AActor* TargetNPC)
{
    if (!TargetNPC) return;

    // C. Synchronized Finisher (Doga-Style)
    ApplyHitStop(0.2f); // Long freeze for impact
    TriggerImpactShake(3.0f); // High-intensity vibration

    // Warp target into position (Motion Warping)
    // Execute finishers...
    
    UE_LOG(LogTemp, Error, TEXT("VIGILANTE: Doga-Style Finisher Executed on %s"), *TargetNPC->GetName());
}

void UHeroAbility_Brawler::ApplyHitStop(float Duration)
{
    // PAUSE: Instantaneous hit-stop for visceral feel
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.1f);
    
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
    {
        UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
    }), Duration, false);
}

void UHeroAbility_Brawler::TriggerImpactShake(float Intensity)
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (PC && HeavyImpactShakeClass)
    {
        PC->ClientStartCameraShake(HeavyImpactShakeClass, Intensity);
    }
}

void UHeroAbility_Brawler::ResetCombo()
{
    ComboCounter = 0;
}
