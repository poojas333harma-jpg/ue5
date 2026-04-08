// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HeroCoreTypes.h"
#include "HeroLanguageManager.generated.h"

/**
 * AAA MASTER LANGUAGE MANAGER
 * Handles Global Asset Swapping (Audio, Text, Texture, Video) 
 * and dynamic language registration.
 */
UCLASS()
class HERO_API UHeroLanguageManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    /** 
     * Registers a naya Language Pack. 
     * '+' Logic: Call this from a DataAsset loader or Folder scanner.
     */
    UFUNCTION(BlueprintCallable, Category = "Hero|Localization")
    void RegisterLanguagePack(EHeroLanguageID LangID, const FHeroLocalizationPack& Pack);

    /** 
     * Global One-Click Switcher. 
     * Swaps all assets and broadcasts the change.
     */
    UFUNCTION(BlueprintCallable, Category = "Hero|Localization")
    void SetActiveLanguage(EHeroLanguageID NewLangID);

    UFUNCTION(BlueprintPure, Category = "Hero|Localization")
    EHeroLanguageID GetCurrentLanguage() const { return CurrentLanguage; }

    // ── Asset Retrieval ─────────────────────────────

    UFUNCTION(BlueprintCallable, Category = "Hero|Localization")
    FText GetLocalizedText(FName Key);

    UFUNCTION(BlueprintCallable, Category = "Hero|Localization")
    class USoundBase* GetLocalizedAudio(FName Key);

    UFUNCTION(BlueprintCallable, Category = "Hero|Localization")
    class UTexture2D* GetLocalizedTexture(FName Key);

    UFUNCTION(BlueprintCallable, Category = "Hero|Localization")
    class UMediaSource* GetLocalizedVideo(FName Key);

    /** Broadcasts when language changes. Binds UI/Signs to this. */
    UPROPERTY(BlueprintAssignable, Category = "Hero|Localization")
    FOnLanguageChanged OnLanguageChanged;

private:
    UPROPERTY()
    EHeroLanguageID CurrentLanguage = EHeroLanguageID::EN;

    UPROPERTY()
    TMap<EHeroLanguageID, FHeroLocalizationPack> LanguageRegistry;

    /** Helper to load soft pointers synchronously for the switcher */
    template<typename T>
    T* LoadLocalizedAsset(const TMap<FName, TSoftObjectPtr<T>>& AssetMap, FName Key);
};
