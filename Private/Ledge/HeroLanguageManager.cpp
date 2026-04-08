// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroLanguageManager.h"
#include "Sound/SoundBase.h"
#include "Engine/Texture2D.h"
#include "MediaSource.h"

void UHeroLanguageManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    // Initialize with English as default
    CurrentLanguage = EHeroLanguageID::EN;
}

void UHeroLanguageManager::RegisterLanguagePack(EHeroLanguageID LangID, const FHeroLocalizationPack& Pack)
{
    LanguageRegistry.Add(LangID, Pack);
    
    // Log registration (Plus Feature)
    UE_LOG(LogTemp, Log, TEXT("Language Registered: %d"), (int32)LangID);
}

void UHeroLanguageManager::SetActiveLanguage(EHeroLanguageID NewLangID)
{
    if (CurrentLanguage != NewLangID)
    {
        CurrentLanguage = NewLangID;
        
        // Broadcast to all UI / Signs / Audio listeners
        OnLanguageChanged.Broadcast();
        
        UE_LOG(LogTemp, Warning, TEXT("Global Language Switched to: %d"), (int32)NewLangID);
    }
}

FText UHeroLanguageManager::GetLocalizedText(FName Key)
{
    if (FHeroLocalizationPack* Pack = LanguageRegistry.Find(CurrentLanguage))
    {
        if (FText* Value = Pack->TextPacks.Find(Key))
        {
            return *Value;
        }
    }
    return FText::FromName(Key); // Fallback
}

USoundBase* UHeroLanguageManager::GetLocalizedAudio(FName Key)
{
    if (FHeroLocalizationPack* Pack = LanguageRegistry.Find(CurrentLanguage))
    {
        return LoadLocalizedAsset<USoundBase>(Pack->AudioPacks, Key);
    }
    return nullptr;
}

UTexture2D* UHeroLanguageManager::GetLocalizedTexture(FName Key)
{
    if (FHeroLocalizationPack* Pack = LanguageRegistry.Find(CurrentLanguage))
    {
        return LoadLocalizedAsset<UTexture2D>(Pack->TexturePacks, Key);
    }
    return nullptr;
}

UMediaSource* UHeroLanguageManager::GetLocalizedVideo(FName Key)
{
    if (FHeroLocalizationPack* Pack = LanguageRegistry.Find(CurrentLanguage))
    {
        return LoadLocalizedAsset<UMediaSource>(Pack->VideoPacks, Key);
    }
    return nullptr;
}

template<typename T>
T* UHeroLanguageManager::LoadLocalizedAsset(const TMap<FName, TSoftObjectPtr<T>>& AssetMap, FName Key)
{
    if (const TSoftObjectPtr<T>* SoftPtr = AssetMap.Find(Key))
    {
        if (SoftPtr->IsPending())
        {
            // For one-click frame switching, we force a synchronous load.
            // In a production environment with massive data, this would be async.
            return SoftPtr->LoadSynchronous();
        }
        return SoftPtr->Get();
    }
    return nullptr;
}
