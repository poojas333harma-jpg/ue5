// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroMultiplayerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

void UHeroMultiplayerSubsystem::HostGame(FName MapName)
{
	if (UWorld* World = GetWorld())
	{
		FString ConnectOptions = TEXT("listen");
		UGameplayStatics::OpenLevel(World, MapName, true, ConnectOptions);
	}
}

void UHeroMultiplayerSubsystem::JoinGame(const FString& IPAddress)
{
	if (APlayerController* PC = GetGameInstance()->GetFirstLocalPlayerController())
	{
		PC->ClientTravel(IPAddress, ETravelType::TRAVEL_Absolute);
	}
}

bool UHeroMultiplayerSubsystem::IsHosting() const
{
    if (UWorld* World = GetWorld())
    {
        return World->GetNetMode() == NM_ListenServer;
    }
    return false;
}
