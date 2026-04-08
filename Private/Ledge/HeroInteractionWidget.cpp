// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroInteractionWidget.h"
#include "GameFramework/Character.h"
#include "Ledge/HeroNPCBase.h"
#include "Ledge/HeroCoreTypes.h"

void UHeroInteractionWidget::InitializeInteraction(AHeroNPCBase* NPC, FHeroNPCInteractionRow InteractionRow)
{
    if (!NPC) return;

    TargetNPC = NPC;
    GreetingText = InteractionRow.DefaultGreeting;

    // Trigger Blueprint logic to build the choice buttons
    OnPopulateOptions(InteractionRow.AvailableOptions);
}

void UHeroInteractionWidget::HandleOptionSelected(EHeroInteractionMenuOption SelectedOption)
{
    if (!TargetNPC) return;

    switch (SelectedOption)
    {
        case EHeroInteractionMenuOption::Talk:
            // logic to trigger a specific dialogue line
            break;

        case EHeroInteractionMenuOption::Trade:
            // call Trade UI
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("OPENING SHOP..."));
            break;

        case EHeroInteractionMenuOption::Steal:
            // opportunistic logic
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("THIEF MODE ACTIVE!"));
            break;

        default:
            break;
    }

    // Close menu after selection (optional)
    // RemoveFromParent();
}
