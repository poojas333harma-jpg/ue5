// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroTradeComponent.h"
#include "Ledge/HeroInventoryComponent.h"
#include "Ledge/HeroItemData.h"
#include "Ledge/HeroNPCBase.h"

UHeroTradeComponent::UHeroTradeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

float UHeroTradeComponent::GetCalculatedPrice(FName ItemID, float Quality, bool bIsBuying)
{
    // Logic: In a full game, we'd fetch the BasePrice from DT_MasterEconomy
    float BasePrice = 100.0f; 
    
    // 1. Decay Affect: Fresh = 100% price, Rotten (Quality 0.2) = 20% price
    float QualityMultiplier = FMath::Clamp(Quality, 0.2f, 1.0f);
    
    // 2. Merchant Profile: Shopkeeper pays more, Scavenger pays less
    float ProfileMultiplier = 1.0f;
    if (MerchantProfile == EHeroNPCProfile::Merchant) ProfileMultiplier = 1.2f;
    if (MerchantProfile == EHeroNPCProfile::Law_Enforcement) ProfileMultiplier = 0.8f;
    
    // 3. Buy/Sell Difference (Spread)
    float FinalPrice = BasePrice * QualityMultiplier * ProfileMultiplier * RegionalPriceMultiplier;
    
    if (!bIsBuying) FinalPrice *= 0.8f; // Sell at discount
    
    return FinalPrice;
}

bool UHeroTradeComponent::ExecuteTrade(UHeroInventoryComponent* PlayerInv, TArray<FHeroItemSlot> PlayerOffer, TArray<FHeroItemSlot> MerchantOffer)
{
    if (!PlayerInv) return false;

    float PlayerValue = 0.0f;
    float MerchantValue = 0.0f;

    // Calculate valuation
    for (const FHeroItemSlot& Slot : PlayerOffer) PlayerValue += GetCalculatedPrice(Slot.ItemID, Slot.CurrentDurability/100.f, false);
    for (const FHeroItemSlot& Slot : MerchantOffer) MerchantValue += GetCalculatedPrice(Slot.ItemID, Slot.CurrentDurability/100.f, true);

    // BARTER LOGIC: If values match (within 5% margin), exchange
    if (PlayerValue >= MerchantValue * 0.95f)
    {
        // 1. Remove from player, add to merchant (and vice-versa)
        // ... (Call PlayerInv->RemoveItem / AddItem)
        
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, TEXT("Economy: BARTER SUCCESSFUL!"));
        return true;
    }

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Economy: BARTER REJECTED - Value mismatch"));
    return false;
}
