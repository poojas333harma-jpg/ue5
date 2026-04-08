// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroJournalComponent.h"
#include "Ledge/HeroItemData.h"

UHeroJournalComponent::UHeroJournalComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UHeroJournalComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UHeroJournalComponent::UnlockLoreEntry(FHeroLoreEntry NewEntry)
{
    // Check if title already exists
    for (const FHeroLoreEntry& Entry : UnlockedLore)
    {
        if (Entry.Title.EqualTo(NewEntry.Title)) return;
    }

    UnlockedLore.Add(NewEntry);
    GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Magenta, FString::Printf(TEXT("JOURNAL UNLOCKED: %s"), *NewEntry.Title.ToString()));
}

TArray<FHeroLoreEntry> UHeroJournalComponent::GetLoreByTag(FGameplayTag Tag)
{
    TArray<FHeroLoreEntry> Results;
    for (const FHeroLoreEntry& Entry : UnlockedLore)
    {
        if (Entry.TopicTag == Tag) Results.Add(Entry);
    }
    return Results;
}

void UHeroJournalComponent::UnlockLoreFromItem(UHeroItemData* Item)
{
    if (!Item) return;
    UnlockLoreEntry(Item->LoreData);
}
