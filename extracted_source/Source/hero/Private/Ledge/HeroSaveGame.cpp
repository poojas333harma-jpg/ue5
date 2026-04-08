// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroSaveGame.h"

UHeroSaveGame::UHeroSaveGame()
{
    SlotName = TEXT("HeroSlot_0");
    UserIndex = 0;
    LastWeatherType = EHeroWeatherType::Clear;
}
