// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HeroCoreTypes.h"
#include "HeroTradeComponent.generated.h"

/**
 * AAA TRADING & ECONOMY COMPONENT
 * Handles Merchant profiles, Dynamic Pricing, and Barter Logic.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HERO_API UHeroTradeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHeroTradeComponent();

    /** 
     * Calculates the price for an item based on merchant profile and item state. 
     * @param ItemID - The item ID to check.
     * @param Quality - 0.0 to 1.0 (Affected by Decay).
     */
    UFUNCTION(BlueprintCallable, Category = "Hero|Economy")
    float GetCalculatedPrice(FName ItemID, float Quality, bool bIsBuying);

    /**
     * Executes a barter transaction between the player and this merchant.
     */
    UFUNCTION(BlueprintCallable, Category = "Hero|Economy")
    bool ExecuteTrade(class UHeroInventoryComponent* PlayerInv, TArray<FHeroItemSlot> PlayerOffer, TArray<FHeroItemSlot> MerchantOffer);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Economy")
    EHeroNPCProfile MerchantProfile = EHeroNPCProfile::Merchant;

    /** Regional price multiplier (e.g. 1.2x in warzones) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Economy")
    float RegionalPriceMultiplier = 1.0f;

private:
    float GetPriceAdjustmentForProfile(EHeroItemRarity Rarity);
};
