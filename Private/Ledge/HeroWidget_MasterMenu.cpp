// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroWidget_MasterMenu.h"
#include "Components/WidgetSwitcher.h"
#include "GameFramework/Character.h"
#include "Ledge/HeroInventoryComponent.h"
#include "Ledge/HeroProgressionComponent.h"
#include "Ledge/HeroQuestComponent.h"

void UHeroWidget_MasterMenu::NativeConstruct()
{
    Super::NativeConstruct();

    // 1. Find the Player Character
    APawn* PlayerPawn = GetOwningPlayerPawn();
    if (PlayerPawn)
    {
        InventoryComp = PlayerPawn->FindComponentByClass<UHeroInventoryComponent>();
        ProgressionComp = PlayerPawn->FindComponentByClass<UHeroProgressionComponent>();
        QuestComp = PlayerPawn->FindComponentByClass<UHeroQuestComponent>();
    }
}

void UHeroWidget_MasterMenu::SwitchToTab(int32 Index)
{
    if (TabSwitcher && TabSwitcher->GetNumWidgets() > Index)
    {
        TabSwitcher->SetActiveWidgetIndex(Index);
    }
}

void UHeroWidget_MasterMenu::NextTab()
{
    if (TabSwitcher)
    {
        int32 NextIndex = (TabSwitcher->GetActiveWidgetIndex() + 1) % TabSwitcher->GetNumWidgets();
        SwitchToTab(NextIndex);
    }
}

void UHeroWidget_MasterMenu::PreviousTab()
{
    if (TabSwitcher)
    {
        int32 PrevIndex = TabSwitcher->GetActiveWidgetIndex() - 1;
        if (PrevIndex < 0) PrevIndex = TabSwitcher->GetNumWidgets() - 1;
        SwitchToTab(PrevIndex);
    }
}
