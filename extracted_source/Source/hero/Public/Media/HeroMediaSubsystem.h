// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MediaPlayer.h"
#include "MediaTexture.h"
#include "HeroMediaSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FHeroPooledMedia
{
    GENERATED_BODY()

    UPROPERTY()
    TObjectPtr<UMediaPlayer> Player;

    UPROPERTY()
    TObjectPtr<UMediaTexture> Texture;

    bool bInUse = false;
};

/**
 * AAA MEDIA SUBSYSTEM
 * Handles global pooling of MediaPlayers and Textures to prevent memory spikes
 * during large-scale fish schools (Phase 20-F) and NPC interactions (Phase 22-B).
 */
UCLASS()
class HERO_API UHeroMediaSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    /** Acquires a free media set from the pool. Creates one if none are available. */
    UFUNCTION(BlueprintCallable, Category = "Hero|Media")
    void AcquireMediaSet(UMediaPlayer*& OutPlayer, UMediaTexture*& OutTexture);

    /** Returns a media set back to the pool for reuse. */
    UFUNCTION(BlueprintCallable, Category = "Hero|Media")
    void ReleaseMediaSet(UMediaPlayer* InPlayer);

private:
    UPROPERTY()
    TArray<FHeroPooledMedia> MediaPool;

    int32 MaxPoolSize = 32;
};
