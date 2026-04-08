// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Ledge/HeroCoreTypes.h"
#include "HeroInteractionWidget.generated.h"

/**
 * DYNAMIC INTERACTION WIDGET
 * Populates choices based on NPC Profile and Data Table.
 */
UCLASS()
class HERO_API UHeroInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    /** 
     * Initializes the menu for a specific NPC.
     * @param NPC - The actor we are talking to.
     * @param InteractionRow - Data from the global Data Table.
     */
    UFUNCTION(BlueprintCallable, Category = "Hero|Dialogue")
    void InitializeInteraction(class AHeroNPCBase* NPC, FHeroNPCInteractionRow InteractionRow);

    /** Called when a user clicks an option (Talk, Trade, etc.) */
    UFUNCTION(BlueprintCallable, Category = "Hero|Dialogue")
    void HandleOptionSelected(EHeroInteractionMenuOption SelectedOption);

protected:
    /** 
     * Blueprint Event to actually create the buttons in the UI. 
     * Handled in WBP_InteractionMenu.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Hero|Dialogue")
    void OnPopulateOptions(const TArray<EHeroInteractionMenuOption>& Options);

    UPROPERTY(BlueprintReadOnly, Category = "Hero|Dialogue")
    class AHeroNPCBase* TargetNPC;

    UPROPERTY(BlueprintReadOnly, Category = "Hero|Dialogue")
    FText GreetingText;
};
