// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HeroMultiplayerSubsystem.generated.h"

/**
 * HERO MULTIPLAYER SUBSYSTEM
 * Handles Listen Server hosting and Direct IP joining for 2-PC LAN connectivity.
 */
UCLASS()
class HERO_API UHeroMultiplayerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/** Starts a Listen Server on the current map */
	UFUNCTION(BlueprintCallable, Category = "Hero|Multiplayer")
	void HostGame(FName MapName);

	/** Joins a server via IP Address */
	UFUNCTION(BlueprintCallable, Category = "Hero|Multiplayer")
	void JoinGame(const FString& IPAddress);

    /** Returns true if we are currently a Listen Server host */
    UFUNCTION(BlueprintCallable, Category = "Hero|Multiplayer")
    bool IsHosting() const;
};
