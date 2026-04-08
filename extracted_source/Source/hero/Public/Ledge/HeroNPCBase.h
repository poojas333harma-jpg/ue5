// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Ledge/HeroCoreTypes.h"
#include "HeroNPCBase.generated.h"

/**
 * AAA MASTER NPC BASE
 * A unified skeleton for all NPC types (Howlers, Thieves, Allies, Townies).
 * Supports the Profile System, Dynamic Gore, and Persistent World Interaction.
 */
UCLASS()
class HERO_API AHeroNPCBase : public ACharacter
{
	GENERATED_BODY()

public:
	AHeroNPCBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

    // ═════════════════════════════════════════════
    // 1. NPC ARCHETYPE & FACTION
    // ═════════════════════════════════════════════

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Profile")
    EHeroNPCProfile NPCProfile = EHeroNPCProfile::Entertainer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Profile")
    EHeroNPCFaction NPCFaction = EHeroNPCFaction::Neutral;

    UFUNCTION(BlueprintCallable, Category = "NPC")
    void SetAIProfile(EHeroNPCProfile NewProfile);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Profile")
    FText NPCDisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Gender")
    EHeroGender Gender = EHeroGender::Male;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Gender")
    bool bIsFemale = false;

    // ═════════════════════════════════════════════
    // 2. STATE & DEATH
    // ═════════════════════════════════════════════

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC|State")
    bool bIsDead = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC|State")
    bool bIsSearched = false;

    // ═════════════════════════════════════════════
    // 3. AI PERCEPTION (Phase 58 cornerstone)
    // ═════════════════════════════════════════════
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionComponent* PerceptionComponent;

    UFUNCTION()
    void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float BaseSightRadius = 2000.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float BaseLoseSightRadius = 2500.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float FieldOfView = 90.f;

    // ═════════════════════════════════════════════
    // 4. COMPONENTS
    // ═════════════════════════════════════════════

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC|Components")
    class UHeroGoreComponent* GoreComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC|Components")
    class UHeroHealthComponent* HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC|Components")
    class UMotionWarpingComponent* MotionWarpingComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC|Components")
    class UNavigationInvokerComponent* NavigationInvoker;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Boss")
    bool bIsBoss = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Boss", meta = (EditCondition = "bIsBoss"))
    class UDataTable* BossAttributeTable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Boss", meta = (EditCondition = "bIsBoss"))
    FName BossAttributeRow;

    UPROPERTY(BlueprintReadOnly, Category = "NPC|Boss")
    FHeroBossAttributes BossAttributes;

protected:
    UFUNCTION()
    void HandleDamageTaken(float Damage, FVector HitLocation);

public:
    UFUNCTION(BlueprintCallable, Category = "NPC|Death")
    virtual void Die();

    /** Call this after a looting montage ends. */
    UFUNCTION(BlueprintCallable, Category = "NPC|Death")
    void MarkAsSearched();

    UFUNCTION(BlueprintImplementableEvent, Category = "NPC|Logic")
    void OnProfileChanged(EHeroNPCProfile NewProfile);

    /** Reacts to sensory stimuli (Climbing constraint check) */
    UFUNCTION(BlueprintCallable, Category = "NPC|AI")
    bool CanClimbToTarget(FVector TargetLocation);

    /** Master Data Asset for auto-populating AI and GFX references */
    UPROPERTY(EditDefaultsOnly, Category = "NPC|Config")
    class UHeroNPCDataAsset* ConfigurationAsset;

    /** Animation Master Tables (Enemy, Civilian, Ally) */
    UPROPERTY(EditDefaultsOnly, Category = "NPC|Animation")
    class UDataTable* AnimationDataTable;

    UPROPERTY(BlueprintReadOnly, Category = "NPC|Animation")
    FHeroAnimSet CurrentAnimSet;

    /** Current Visibility Scale (1.0f = Light, 0.0f = Dark) */
    UPROPERTY(BlueprintReadOnly, Category = "NPC|State")
    float CurrentVisibilityMeter = 1.0f;

protected:
    void SetupProfileStats();
};
