// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroNPCBase.h"
#include "Ledge/HeroGoreComponent.h"
#include "Ledge/HeroHealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MotionWarpingComponent.h"
#include "NavigationInvokerComponent.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Ledge/HeroLightSensorComponent.h"
#include "Ledge/HeroAIController.h"
#include "Ledge/HeroNPCDataAsset.h"
#include "Ledge/HeroCoreTypes.h"
#include "DataTableUtils.h"
#include "Kismet/GameplayStatics.h"
#include "MassAI/HeroMassModule.h"

AHeroNPCBase::AHeroNPCBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Component Setup
	GoreComponent = CreateDefaultSubobject<UHeroGoreComponent>(TEXT("GoreComponent"));
	HealthComponent = CreateDefaultSubobject<UHeroHealthComponent>(TEXT("HealthComponent"));
    MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));

    // --- Phase 19: Dynamic NavMesh Optimization ---
    NavigationInvoker = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavigationInvoker"));
    // Generate NavMesh in a 50m radius, remove at 70m
    NavigationInvoker->SetGenerationRadii(5000.f, 7000.f);

    // --- Phase 58: Perception Integration ---
    PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
    
    // Core Sight Configuration
    UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    SightConfig->SightRadius = BaseSightRadius;
    SightConfig->LoseSightRadius = BaseLoseSightRadius;
    SightConfig->PeripheralVisionAngleDegrees = FieldOfView;
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

    PerceptionComponent->ConfigureSense(*SightConfig);
    PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
    PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AHeroNPCBase::OnPerceptionUpdated);

    // 2. Assign AI Controller
    AIControllerClass = AHeroAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AHeroNPCBase::BeginPlay()
{
	Super::BeginPlay();
	
    if (ConfigurationAsset)
    {
        ConfigurationAsset->PopulateNPC(this, NPCProfile);
    }

    if (AnimationDataTable)
    {
        const FName RowName = FName(*UEnum::GetValueAsString(NPCProfile));
        FHeroAnimSet* FoundSet = AnimationDataTable->FindRow<FHeroAnimSet>(RowName, TEXT("NPC Anim Loading"));

        if (FoundSet)
        {
            CurrentAnimSet = *FoundSet;
            bIsFemale = CurrentAnimSet.bIsFemale;
            
            if (CurrentAnimSet.PhysicsAssetOverride)
            {
                GetMesh()->SetPhysicsAsset(CurrentAnimSet.PhysicsAssetOverride);
            }
        }
    }

    SetupProfileStats();

    if (bIsBoss && BossAttributeTable && !BossAttributeRow.IsNone())
    {
        FHeroBossAttributes* FoundAttributes = BossAttributeTable->FindRow<FHeroBossAttributes>(BossAttributeRow, TEXT("Boss Attribute Loading"));
        if (FoundAttributes)
        {
            BossAttributes = *FoundAttributes;
            if (HealthComponent)
            {
                HealthComponent->MaxHealth = BossAttributes.MaxHealth;
                HealthComponent->CurrentHealth = BossAttributes.MaxHealth;
            }
            NPCDisplayName = BossAttributes.BossName;
        }
    }

    if (HealthComponent)
    {
        HealthComponent->OnDamageTaken.AddDynamic(this, &AHeroNPCBase::HandleDamageTaken);
    }
}

void AHeroNPCBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // DYNAMIC RADIUS MODULATION (Phase 58)
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (PlayerPawn && PerceptionComponent)
    {
        UHeroLightSensorComponent* LightSensor = PlayerPawn->FindComponentByClass<UHeroLightSensorComponent>();
        if (LightSensor)
        {
            // NEW: Modulate Sight Radius based on Player Visibility Scalar
            UAISenseConfig_Sight* SightConfig = Cast<UAISenseConfig_Sight>(PerceptionComponent->GetSenseConfig(UAISense_Sight::StaticClass()->GetFName()));
            if (SightConfig)
            {
                float VisibilityScale = LightSensor->VisibilityScalar;
                float NewRadius = BaseSightRadius * VisibilityScale;
                
                SightConfig->SightRadius = FMath::FInterpTo(SightConfig->SightRadius, NewRadius, DeltaTime, 2.0f);
                PerceptionComponent->ConfigureSense(*SightConfig);
            }
        }
    }

    CurrentVisibilityMeter = FMath::FInterpTo(CurrentVisibilityMeter, 1.0f, DeltaTime, 2.0f);

    if (AHeroAIController* AIC = Cast<AHeroAIController>(GetController()))
    {
        float VisibilityMultiplier = FMath::Clamp(CurrentVisibilityMeter, 0.5f, 1.0f);
        AIC->UpdatePerceptionByLight(VisibilityMultiplier);
    }
}

void AHeroNPCBase::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
    for(AActor* Actor : UpdatedActors)
    {
        if (Actor->ActorHasTag(TEXT("Player")))
        {
            // PLAYER DETECTED: Trigger Mass Aggression Broadcast (Phase 58)
            UHeroMassModule::TriggerGlobalAlert(Actor->GetActorLocation());
        }
    }
}

void AHeroNPCBase::SetAIProfile(EHeroNPCProfile NewProfile)
{
	NPCProfile = NewProfile;
	SetupProfileStats();
	OnProfileChanged(NewProfile);
}

void AHeroNPCBase::SetupProfileStats()
{
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (!MoveComp) return;

	switch (NPCProfile)
	{
	case EHeroNPCProfile::Link:
		MoveComp->MaxWalkSpeed = 450.f;
		break;
	case EHeroNPCProfile::Brute:
		MoveComp->MaxWalkSpeed = 300.f;
		break;
	case EHeroNPCProfile::Thief:
	case EHeroNPCProfile::Scavenger:
		MoveComp->MaxWalkSpeed = 550.f;
		break;
	case EHeroNPCProfile::Elite:
	case EHeroNPCProfile::Assault_Squad:
		MoveComp->MaxWalkSpeed = 400.f;
		break;
	default:
		MoveComp->MaxWalkSpeed = 350.f;
		break;
	}
}

void AHeroNPCBase::Die()
{
	if (bIsDead) return;

	bIsDead = true;
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->DisableMovement();
	}

	UE_LOG(LogTemp, Warning, TEXT("NPC %s has Died."), *GetName());
}

void AHeroNPCBase::MarkAsSearched()
{
    bIsSearched = true;
    if (GoreComponent)
    {
        GoreComponent->StartPostMortemDissolve(5.0f);
    }
}

bool AHeroNPCBase::CanClimbToTarget(FVector TargetLocation)
{
    float HeightDiff = FMath::Abs(TargetLocation.Z - GetActorLocation().Z);
    if (NPCProfile == EHeroNPCProfile::Link && HeightDiff > 400.f)
    {
        return false;
    }
    return true; 
}

void AHeroNPCBase::HandleDamageTaken(float Damage, FVector HitLocation)
{
    UE_LOG(LogTemp, Log, TEXT("DAMAGE POP-UP: -%.0f at %s"), Damage, *HitLocation.ToString());
}
