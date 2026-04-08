// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Ledge/HeroNPCBase.h"
#include "Ledge/HeroNPCDataAsset.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"

AHeroAIController::AHeroAIController()
{
	HeroPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("HeroPerception"));
	
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	HearingConfig =CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));

	if (HeroPerception)
	{
		 HeroPerception->ConfigureSense(*SightConfig);
		 HeroPerception->ConfigureSense(*HearingConfig);
		 HeroPerception->SetDominantSense(SightConfig->GetSenseImplementation());

		 HeroPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AHeroAIController::OnPerceptionUpdated);
	}
}

void AHeroAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AHeroAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

    if (AHeroNPCBase* NPC = Cast<AHeroNPCBase>(InPawn))
    {
        // Fetch config from the NPC's data asset
        if (NPC->ConfigurationAsset)
        {
            FHeroNPCProfileConfig Config;
            bool bSuccess = false;
            NPC->ConfigurationAsset->GetConfigForProfile(NPC->NPCProfile, bSuccess, Config);
            
            if (bSuccess)
            {
                BasePerception = Config.PerceptionConfig;

                // Apply to Perception Configs
                SightConfig->SightRadius = BasePerception.SightRadius;
                SightConfig->LoseSightRadius = BasePerception.LoseSightRadius;
                SightConfig->PeripheralVisionAngleDegrees = BasePerception.PeripheralVisionAngle;
                
                HearingConfig->HearingRange = BasePerception.HearingRange;

                HeroPerception->ConfigureSense(*SightConfig);
                HeroPerception->ConfigureSense(*HearingConfig);
            }
        }
    }
}

void AHeroAIController::UpdatePerceptionByLight(float LightMultiplier)
{
    if (SightConfig && HeroPerception)
    {
        // LightMultiplier (1.0f - 0.5f)
        SightConfig->SightRadius = BasePerception.SightRadius * LightMultiplier;
        SightConfig->LoseSightRadius = BasePerception.LoseSightRadius * LightMultiplier;
        
        HeroPerception->ConfigureSense(*SightConfig);
    }
}

void AHeroAIController::RequestBackup()
{
    // Implementation: Use a sphere trace or a global AI manager to alert allies
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("AI: CALLING FOR BACKUP!"));
}

void AHeroAIController::ExecuteFlanking(AActor* PlayerTarget)
{
    if (!PlayerTarget) return;

    // Logic: Calculate a position 45-90 degrees from the player's front facing
    FVector RightDir = PlayerTarget->GetActorRightVector();
    FVector FlankPos = PlayerTarget->GetActorLocation() + (RightDir * 800.f);
    
    MoveToLocation(FlankPos, 50.f);
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("AI: EXECUTING FLANK MANEUVER"));
}

void AHeroAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    if (Actor && Stimulus.WasSuccessfullySensed())
    {
        // Alert the squad!
    }
}
