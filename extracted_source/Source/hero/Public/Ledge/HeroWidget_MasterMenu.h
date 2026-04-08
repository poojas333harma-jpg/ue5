// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HeroWidget_MasterMenu.generated.h"

class UWidgetSwitcher;
class UHeroInventoryComponent;
class UHeroProgressionComponent;
class UHeroQuestComponent;

/**
 * AAA MASTER MENU WIDGET (Tabs System)
 * Manages Switching between Skills, Inventory, Quests, and Journal.
 */
UCLASS(Abstract, Blueprintable, meta=(DisplayName="Hero Master Menu"))
class HERO_API UHeroWidget_MasterMenu : public UUserWidget
{
    GENERATED_BODY()

public:
    // ═════════════════════════════════════════════
    // 1. COMPONENTS (To be bound in BP)
    // ═════════════════════════════════════════════
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="Menu")
    TObjectPtr<UWidgetSwitcher> TabSwitcher;

    // ═════════════════════════════════════════════
    // 2. DATA BINDING
    // ═════════════════════════════════════════════
    UPROPERTY(BlueprintReadOnly, Category="Menu")
    TObjectPtr<UHeroInventoryComponent> InventoryComp;

    UPROPERTY(BlueprintReadOnly, Category="Menu")
    TObjectPtr<UHeroProgressionComponent> ProgressionComp;

    UPROPERTY(BlueprintReadOnly, Category="Menu")
    TObjectPtr<UHeroQuestComponent> QuestComp;

    // ═════════════════════════════════════════════
    // 3. TAB LOGIC
    // ═════════════════════════════════════════════
    UFUNCTION(BlueprintCallable, Category="Menu")
    void SwitchToTab(int32 Index);

    UFUNCTION(BlueprintCallable, Category="Menu")
    void NextTab();

    UFUNCTION(BlueprintCallable, Category="Menu")
    void PreviousTab();

    /** Initialization hook */
    virtual void NativeConstruct() override;
};
