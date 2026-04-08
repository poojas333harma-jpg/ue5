// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HeroCoreTypes.h"
#include "HeroJournalComponent.generated.h"

/**
 * AAA JOURNAL & CODEX COMPONENT
 * Manages item lore, bestiary entries, and collectible history.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), meta=(DisplayName="Hero Journal Component"))
class HERO_API UHeroJournalComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHeroJournalComponent();

protected:
    virtual void BeginPlay() override;

public:
    // ═════════════════════════════════════════════
    // 1. DATA STORAGE
    // ═════════════════════════════════════════════
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Journal")
    TArray<FHeroLoreEntry> UnlockedLore;

    // ═════════════════════════════════════════════
    // 2. CORE LOGIC
    // ═════════════════════════════════════════════
    UFUNCTION(BlueprintCallable, Category="Journal")
    void UnlockLoreEntry(FHeroLoreEntry NewEntry);

    UFUNCTION(BlueprintCallable, Category="Journal")
    TArray<FHeroLoreEntry> GetLoreByTag(FGameplayTag Tag);

    /** Called automatically when a legendary item is picked up */
    UFUNCTION(BlueprintCallable, Category="Journal")
    void UnlockLoreFromItem(class UHeroItemData* Item);
};
