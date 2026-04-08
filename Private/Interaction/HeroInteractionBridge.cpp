// Copyright 2026 Hero Project. All Rights Reserved.

#include "Interaction/HeroInteractionBridge.h"
#include "Media/HeroMediaSubsystem.h"
#include "Ledge/HeroNPCBase.h"
#include "Ledge/AHeroWeatherManager.h"
#include "MediaPlayer.h"
#include "MediaTexture.h"
#include "MediaSource.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

AHeroInteractionBridge::AHeroInteractionBridge()
{
	PrimaryActorTick.bCanEverTick = true;

    MediaPlate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MediaPlate"));
    RootComponent = MediaPlate;
    MediaPlate->SetVisibility(false);
}

void AHeroInteractionBridge::BeginPlay()
{
	Super::BeginPlay();
	
    // 1. Acquire Media from Pool
    if (UHeroMediaSubsystem* MediaSS = GetWorld()->GetSubsystem<UHeroMediaSubsystem>())
    {
        UMediaPlayer* TempPlayer = nullptr;
        UMediaTexture* TempTexture = nullptr;
        MediaSS->AcquireMediaSet(TempPlayer, TempTexture);
        ActivePlayer = TempPlayer;
        ActiveTexture = TempTexture;
    }

    // 2. Setup DMI
    if (UMaterialInterface* BaseMat = MediaPlate->GetMaterial(0))
    {
        PlateDMI = MediaPlate->CreateDynamicMaterialInstance(0, BaseMat);
        if (PlateDMI && ActiveTexture)
        {
            PlateDMI->SetTextureParameterValue(TEXT("VideoTexture"), ActiveTexture);
        }
    }

    // 3. Bind playback finish
    if (ActivePlayer)
    {
        ActivePlayer->OnEndReached.AddDynamic(this, &AHeroInteractionBridge::OnCinematicFinished);
    }
}

void AHeroInteractionBridge::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (UHeroMediaSubsystem* MediaSS = GetWorld()->GetSubsystem<UHeroMediaSubsystem>())
    {
        MediaSS->ReleaseMediaSet(ActivePlayer);
    }

    Super::EndPlay(EndPlayReason);
}

void AHeroInteractionBridge::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (bIsCinematicActive)
    {
        SyncEnvironmentToMaterial();
    }
}

void AHeroInteractionBridge::TriggerCinematicInteraction()
{
    AHeroNPCBase* NPC = TargetNPC.Get();
    if (!NPC || !ActivePlayer || !CinematicSource) return;

    // A. Pre-Open the Media (Zero-Latency Prep)
    ActivePlayer->OpenSource(CinematicSource.Get());
    
    // B. SWAP: 3D NPC Mesh OFF -> Alpha-Media ON
    NPC->GetMesh()->SetVisibility(false);
    MediaPlate->SetVisibility(true);
    
    bIsCinematicActive = true;
    
    UE_LOG(LogTemp, Warning, TEXT("CINEMATIC INTERACTION: Swapping NPC to Alpha-Media Plate."));
}

void AHeroInteractionBridge::RevertTo3DMesh()
{
    AHeroNPCBase* NPC = TargetNPC.Get();
    if (!NPC) return;

    NPC->GetMesh()->SetVisibility(true);
    MediaPlate->SetVisibility(false);
    
    bIsCinematicActive = false;
    
    if (ActivePlayer) ActivePlayer->Close();
}

void AHeroInteractionBridge::SyncEnvironmentToMaterial()
{
    // C. Cold-Environment Post-Process Sync
    // Find the Weather Manager in the world
    AActor* WeatherActor = UGameplayStatics::GetActorOfClass(this, AHeroWeatherManager::StaticClass());
    if (AHeroWeatherManager* WeatherMgr = Cast<AHeroWeatherManager>(WeatherActor))
    {
        if (PlateDMI)
        {
            // Set Material Parameters to match the world's 8000K cold look
            PlateDMI->SetScalarParameterValue(TEXT("WorldIntensity"), WeatherMgr->MasterWeatherIntensity);
            
            // Apply a cold blue tint if the weather is overcast/cold
            FLinearColor ColdTint = (WeatherMgr->CurrentWeather == EHeroWeatherType::Storm) 
                                    ? FLinearColor(0.7f, 0.85f, 1.0f) 
                                    : FLinearColor(1.0f, 1.0f, 1.0f);
                                    
            PlateDMI->SetVectorParameterValue(TEXT("GlobalTint"), ColdTint);
        }
    }
}

void AHeroInteractionBridge::OnCinematicFinished()
{
    RevertTo3DMesh();
    UE_LOG(LogTemp, Log, TEXT("CINEMATIC FINISHED: Returning to 3D Interaction state."));
}
