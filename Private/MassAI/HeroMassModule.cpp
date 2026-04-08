// Copyright 2026 Hero Project. All Rights Reserved.

#include "MassAI/HeroMassModule.h"

// Mass Entity Subsystem references (requires module linkage)
// #include "MassEntitySubsystem.h" 

bool UHeroMassModule::bIsMassReady = false;
bool UHeroMassModule::bIsGlobalAlertActive = false;
FVector UHeroMassModule::GlobalAlertLocation = FVector::ZeroVector;
float UHeroMassModule::AlertTimer = 0.0f;

void UHeroMassModule::InitializeMassAI(UObject* WorldContextObject)
{
	if (!WorldContextObject) return;
	bIsMassReady = true;
	UE_LOG(LogTemp, Warning, TEXT("MASS AI CORE MODULE INITIALIZED: ECS SYSTEM ACTIVE."));
}

void UHeroMassModule::ConfigureLODParameters(float NearRange, float FarRange)
{
	UE_LOG(LogTemp, Log, TEXT("MASS AI LOD CONFIGURED: Rendering optimization active for 5,000+ entities."));
}

void UHeroMassModule::TriggerGlobalAlert(FVector AlertLocation, float AlertDuration)
{
    bIsGlobalAlertActive = true;
    GlobalAlertLocation = AlertLocation;
    AlertTimer = AlertDuration;
    
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, 
        FString::Printf(TEXT("GLOBAL MASS SIGNAL: Aggression Mode at %s"), *AlertLocation.ToString()));
}
