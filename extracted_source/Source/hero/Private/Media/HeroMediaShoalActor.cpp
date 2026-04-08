// Copyright 2026 Hero Project. All Rights Reserved.

#include "Media/HeroMediaShoalActor.h"
#include "Media/HeroMediaSubsystem.h"
#include "MediaPlayer.h"
#include "MediaTexture.h"
#include "MediaSource.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"

AHeroMediaShoalActor::AHeroMediaShoalActor()
{
	PrimaryActorTick.bCanEverTick = true;

    ShoalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShoalMesh"));
    RootComponent = ShoalMesh;

    // Default configuration
    MaxVisibilityDistance = 2500.f;
    FadeSpeed = 3.f;
}

void AHeroMediaShoalActor::BeginPlay()
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
    if (UMaterialInterface* BaseMat = ShoalMesh->GetMaterial(0))
    {
        ShoalDMI = ShoalMesh->CreateDynamicMaterialInstance(0, BaseMat);
        if (ShoalDMI && ActiveTexture)
        {
            ShoalDMI->SetTextureParameterValue(TEXT("VideoTexture"), ActiveTexture);
        }
    }

    // 3. Initial Playback
    CycleShoalAsset();
}

void AHeroMediaShoalActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // 4. Return Media to Pool
    if (UHeroMediaSubsystem* MediaSS = GetWorld()->GetSubsystem<UHeroMediaSubsystem>())
    {
        MediaSS->ReleaseMediaSet(ActivePlayer);
    }

    Super::EndPlay(EndPlayReason);
}

void AHeroMediaShoalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    ApplyDistanceFading(DeltaTime);
}

void AHeroMediaShoalActor::ApplyDistanceFading(float DeltaTime)
{
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
    if (!PlayerPawn || !ShoalDMI) return;

    float Distance = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());
    
    // Smooth Opacity Fading (0.0 at MaxDistance, 1.0 when close)
    float TargetOpacity = FMath::Clamp(1.0f - (Distance / MaxVisibilityDistance), 0.0f, 1.0f);
    
    // Depth Illusion: Fade out if too close as well (Alpha Video limitation)
    if (Distance < 200.f) TargetOpacity *= (Distance / 200.f);

    CurrentOpacity = FMath::FInterpTo(CurrentOpacity, TargetOpacity, DeltaTime, FadeSpeed);
    ShoalDMI->SetScalarParameterValue(TEXT("Opacity"), CurrentOpacity);

    // Memory Guard: Close player if completely invisible for a while
    if (CurrentOpacity < 0.01f && ActivePlayer && ActivePlayer->IsPlaying())
    {
        ActivePlayer->Pause();
    }
    else if (CurrentOpacity > 0.01f && ActivePlayer && !ActivePlayer->IsPlaying())
    {
        ActivePlayer->Play();
    }
}

void AHeroMediaShoalActor::CycleShoalAsset()
{
    if (ShoalAssets.Num() == 0 || !ActivePlayer) return;

    int32 RandomIndex = FMath::RandRange(0, ShoalAssets.Num() - 1);
    ActivePlayer->OpenSource(ShoalAssets[RandomIndex]);
    ActivePlayer->SetLooping(true);
}
