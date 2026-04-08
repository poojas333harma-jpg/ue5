// Copyright 2026 Hero Project. All Rights Reserved.

#include "Media/HeroMediaSubsystem.h"
#include "MediaPlayer.h"
#include "MediaTexture.h"

void UHeroMediaSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

    // Initial pool allocation
    for (int32 i = 0; i < 4; i++) // Start small, grows on demand
    {
        FHeroPooledMedia NewSet;
        NewSet.Player = NewObject<UMediaPlayer>(this);
        NewSet.Texture = NewObject<UMediaTexture>(this);
        NewSet.Texture->SetMediaPlayer(NewSet.Player);
        MediaPool.Add(NewSet);
    }

	UE_LOG(LogTemp, Warning, TEXT("MEDIA SUBSYSTEM INITIALIZED: Pool size 4"));
}

void UHeroMediaSubsystem::Deinitialize()
{
	Super::Deinitialize();
    MediaPool.Empty();
}

void UHeroMediaSubsystem::AcquireMediaSet(UMediaPlayer*& OutPlayer, UMediaTexture*& OutTexture)
{
    for (auto& Media : MediaPool)
    {
        if (!Media.bInUse)
        {
            Media.bInUse = true;
            OutPlayer = Media.Player;
            OutTexture = Media.Texture;
            return;
        }
    }

    // Dynamic growth if within MaxPoolSize
    if (MediaPool.Num() < MaxPoolSize)
    {
        FHeroPooledMedia NewSet;
        NewSet.Player = NewObject<UMediaPlayer>(this);
        NewSet.Texture = NewObject<UMediaTexture>(this);
        NewSet.Texture->SetMediaPlayer(NewSet.Player);
        NewSet.bInUse = true;

        MediaPool.Add(NewSet);
        OutPlayer = NewSet.Player;
        OutTexture = NewSet.Texture;

        UE_LOG(LogTemp, Log, TEXT("MEDIA POOL EXPANDED: Current size %d"), MediaPool.Num());
        return;
    }

    UE_LOG(LogTemp, Error, TEXT("MEDIA POOL OVERFLOW: Unable to acquire a media set!"));
    OutPlayer = nullptr;
    OutTexture = nullptr;
}

void UHeroMediaSubsystem::ReleaseMediaSet(UMediaPlayer* InPlayer)
{
    for (auto& Media : MediaPool)
    {
        if (Media.Player == InPlayer)
        {
            Media.bInUse = false;
            if (Media.Player)
            {
                Media.Player->Close();
            }
            return;
        }
    }
}
