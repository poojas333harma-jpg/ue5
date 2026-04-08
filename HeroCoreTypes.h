// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "HeroCoreTypes.generated.h"

class AActor;

/** --- NAVIGATION & MAP (Phase 17) --- */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractionSuccess, AActor*, Interactor, AActor*, TargetActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLanguageChanged);

UENUM(BlueprintType)
enum class EHeroLanguageID : uint8
{
	HERO_LANGUAGE_NONE = 0 UMETA(DisplayName = "None"),
	EN UMETA(DisplayName = "English"),
	HI UMETA(DisplayName = "Hindi"),
	RU UMETA(DisplayName = "Russian"),
	JP UMETA(DisplayName = "Japanese"),
	CO UMETA(DisplayName = "Cinematic Cold (Phase 20-B)")
};

UENUM(BlueprintType)
enum class EHeroDestructionType : uint8
{
	HERO_DESTRUCT_NONE = 0 UMETA(DisplayName = "None"),
	General,
    Hydrant,
    Pole,
    Electric,
    Flesh,
    Structural
};

USTRUCT(BlueprintType)
struct FHeroLocalizationPack
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, FText> TextPacks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, TSoftObjectPtr<class USoundBase>> AudioPacks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, TSoftObjectPtr<class UTexture2D>> TexturePacks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, TSoftObjectPtr<class UMediaSource>> VideoPacks;
};

UENUM(BlueprintType)
enum class EHeroPickupType : uint8
{
	HERO_PICKUP_NONE = 0 UMETA(DisplayName = "None"),
	Quick,
	LongPress,
	Mashed,
	StaminaLinked,
	CinematicSwap
};

UENUM(BlueprintType)
enum class EHeroSecurityType : uint8
{
	HERO_SECURITY_NONE = 0 UMETA(DisplayName = "None"),
	Lockpick,
	PhysicalForce,
	Electronic,
	Mechanical,
	Biometric,
	Magic
};

UENUM(BlueprintType)
enum class ELockStatus : uint8
{
	HERO_LOCK_STATUS_NONE = 0 UMETA(DisplayName = "None"),
	Locked,
	Open,
	Unlocking,
	Jammed,
	Broken
};

UENUM(BlueprintType)
enum class EHeroMenuOption : uint8
{
	HERO_MENU_NONE = 0 UMETA(DisplayName = "None"),
	Overview,
	Inventory,
	Skills,
    Map,
	Journal,
	Settings
};

UENUM(BlueprintType)
enum class EHeroInteractionMenuOption : uint8
{
	HERO_INTERACT_NONE = 0 UMETA(DisplayName = "None"),
	Talk,
	Trade,
	Quest,
	Recruit,
	Dismiss,
	Attack,
	Steal
};

USTRUCT(BlueprintType)
struct FHeroNPCInteractionRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Greeting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DefaultGreeting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EHeroInteractionMenuOption> AvailableOptions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USoundBase> GreetingAudio;
};

USTRUCT(BlueprintType)
struct FHeroQuestObjective
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsCompleted = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Progress = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TargetCount = 1;

	bool IsComplete() const { return bIsCompleted || CurrentCount >= TargetCount; }
};

UENUM(BlueprintType)
enum class EHeroQuestPriority : uint8
{
	HERO_QUEST_PRIORITY_NONE = 0 UMETA(DisplayName = "None"),
	Minor,
	Major,
	Story,
	Side,
	Endgame
};

USTRUCT(BlueprintType)
struct FHeroMashingData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentProgress = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RequiredForce = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DecayRate = 10.f;
};

UENUM(BlueprintType)
enum class EHeroLockDifficulty : uint8
{
	HERO_DIFFICULTY_NONE = 0 UMETA(DisplayName = "None"),
	Easy,
	Medium,
	Hard,
	Expert,
	Master,
	Legendary
};

UENUM(BlueprintType)
enum class EHeroLockStatus : uint8
{
	HERO_LOCK_STATE_NONE = 0 UMETA(DisplayName = "None"),
	Unlocked,
	Locked,
	Jammed,
	Broken,
	Electronic
};

UENUM(BlueprintType)
enum class EHeroMapViewMode : uint8
{
    HERO_MAP_VIEW_NONE = 0 UMETA(DisplayName = "None"),
    MAP_Standard UMETA(DisplayName = "Standard"),
	MAP_Satellite UMETA(DisplayName = "Satellite"),
	MAP_Thermal UMETA(DisplayName = "Thermal")
};

UENUM(BlueprintType)
enum class EHeroOverlayState : uint8
{
	HERO_OVERLAY_NONE = 0 UMETA(DisplayName = "None"),
	Unarmed,
	Pistol,
	Rifle,
	Sword,
	Bow,
	Torch,
	CarryHeavy
};

USTRUCT(BlueprintType)
struct FOverlayMontageSet
{
	GENERATED_BODY()

	FOverlayMontageSet()
		: ActionStart(nullptr)
		, ActionLoop(nullptr)
		, ActionEnd(nullptr)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* ActionStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimSequence* ActionLoop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* ActionEnd;
};

UENUM(BlueprintType)
enum class EHeroTrailType : uint8
{
	HERO_TRAIL_NONE = 0 UMETA(DisplayName = "None"),
	TRAIL_Footprint,
	TRAIL_Road,
	TRAIL_Water
};

USTRUCT(BlueprintType)
struct FHeroNavigationData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TargetLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EHeroTrailType TrailType = EHeroTrailType::TRAIL_Road;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShowTrail = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DistanceToTarget = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector GoalLocation = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RemainingDistance = 0.f;
};

USTRUCT(BlueprintType)
struct FHero3DMapData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ZoomLevel = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D MapLocation = FVector2D::ZeroVector;
};

/** --- SKILLS (Phase 11) --- */

UENUM(BlueprintType)
enum class EHeroSkillCategory : uint8
{
    HERO_SKILL_CATEGORY_NONE = 0 UMETA(DisplayName = "None"),
    Combat, Parkour, Survival, Divine
};

/** --- COMBAT (Phase 23-B) --- */

UENUM(BlueprintType)
enum class EHeroWeaponType : uint8
{
	HERO_WEAPON_NONE = 0 UMETA(DisplayName = "None"),
	OneHanded_Sword,
	TwoHanded_Axe,
	Greatsword,
	Dagger,
	Mace,
	Bow,
	Crossbow,
	Pistol,
	Rifle,
	Shotgun,
	Staff,
	Fists
};

UENUM(BlueprintType)
enum class EHeroWeaponAttachmentSlot : uint8
{
	HERO_ATTACHMENT_NONE = 0 UMETA(DisplayName = "None"),
	Scope,
	Muzzle,
	Magazine,
	Underbarrel,
	Secondary,
	Module
};

UENUM(BlueprintType)
enum class EHeroAttackType : uint8
{
	HERO_ATTACK_NONE = 0 UMETA(DisplayName = "None"),
	Light,
	Heavy,
	Special,
	Charged,
	Aerial,
	Stealth
};

USTRUCT(BlueprintType)
struct FHeroWeaponModifier
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackSpeedMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StaminaCostMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RangeModifier = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WeightMultiplier = 1.0f;
};

USTRUCT(BlueprintType)
struct FHeroWeaponStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseDamage = 25.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxDurability = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReturnSpeed = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentDurability = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight = 5.f;
};

USTRUCT(BlueprintType)
struct FHeroSurfaceDecals
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UMaterialInterface> ImpactDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector DecalSize = FVector(8.f, 8.f, 8.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LifeSpan = 10.f;
};

UENUM(BlueprintType)
enum class EHeroAttributeType : uint8
{
	HERO_ATTRIBUTE_NONE = 0 UMETA(DisplayName = "None"),
	Health,
	Stamina,
	Grip,
	Oxygen,
	Fear
};

USTRUCT(BlueprintType)
struct FHeroAttributeModifier
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EHeroAttributeType AttributeType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ModifierValue = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsPercent = false;
};

UENUM(BlueprintType)
enum class EHeroAbilityType : uint8
{
	HERO_ABILITY_NONE = 0 UMETA(DisplayName = "None"),
	Movement,
	Combat,
	Social,
	Environment,
	Divine
};

UENUM(BlueprintType)
enum class EHeroNPCProfile : uint8
{
	HERO_NPC_NONE = 0 UMETA(DisplayName = "None"),
	Link,
	Brute,
	Thief,
	Scavenger,
	Elite,
	Assault_Squad,
	Stray,
	Companion,
	Follower,
	Alpha,
	Merchant,
	Law_Enforcement,
	Coward,
	Entertainer,
	Worker,
	Citizen
};

UENUM(BlueprintType)
enum class EHeroNPCFaction : uint8
{
	HERO_FACTION_NONE = 0 UMETA(DisplayName = "None"),
	Neutral,
	Friendly,
	Hostile,
	Guard,
	Merchant,
	Bandit,
	Vigilante
};

USTRUCT(BlueprintType)
struct FHeroBossAttributes : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText BossName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EnrageThreshold = 0.5f;
};

USTRUCT(BlueprintType)
struct FHeroAnimSet : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAnimSequence> Idle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAnimSequence> Walk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAnimSequence> Run;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAnimSequence> Sprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsFemale = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UPhysicsAsset> PhysicsAssetOverride;
};

UENUM(BlueprintType)
enum class EHeroGender : uint8
{
	HERO_GENDER_NONE = 0 UMETA(DisplayName = "None"),
	Male,
	Female,
	NonBinary,
	Other
};

USTRUCT(BlueprintType)
struct FHeroGoreBoneData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BoneName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<class UNiagaraSystem> GoreVFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> GibActorClass;
};

USTRUCT(BlueprintType)
struct FHeroAIPerceptionConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SightRadius = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LoseSightRadius = 3500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PeripheralVisionAngle = 90.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HearingRange = 1500.f;
};

USTRUCT(BlueprintType)
struct FHeroItemSlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentDurability = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CollectionTimeStamp = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SlotIndex = INDEX_NONE;
};

UENUM(BlueprintType)
enum class EHeroItemRarity : uint8
{
	HERO_RARITY_NONE = 0 UMETA(DisplayName = "None"),
	Common,
	Uncommon,
	Rare,
	Epic,
	Legendary,
	Artifact
};

USTRUCT(BlueprintType)
struct FHeroLoreEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(MultiLine=true))
	FText FullText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UTexture2D> Illustration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag TopicTag;
};

USTRUCT(BlueprintType)
struct FHeroItemStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WeightMultiplier = 1.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsQuestItem = false;
};

USTRUCT(BlueprintType)
struct FHeroVehicleSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName VehicleID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform Transform;
};

USTRUCT(BlueprintType)
struct FHeroPlayerSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Rotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHealth = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> UnlockedSkills;
};

USTRUCT(BlueprintType)
struct FAnimalBehaviorSet
{
	GENERATED_BODY()

	FAnimalBehaviorSet()
		: IdleMontage(nullptr)
		, VocalizeMontage(nullptr)
		, IdleBreakMontage(nullptr)
		, ReactionMontage(nullptr)
		, FleeMontage(nullptr)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* IdleMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* VocalizeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* IdleBreakMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* ReactionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* FleeMontage;
};

UENUM(BlueprintType)
enum class EHeroSkillType : uint8
{
    HERO_SKILL_TYPE_NONE = 0 UMETA(DisplayName = "None"),
    Active, Passive, Traversal
};

UENUM(BlueprintType)
enum class EHeroReputationTier : uint8
{
	HERO_REPUTATION_NONE = 0 UMETA(DisplayName = "None"),
	Tier_Unknown,
	Tier_Vigilante,
	Tier_Hero,
	Tier_Legend,
	Neutral,
	Respected,
	Notorious
};

/** --- TRAFFIC & SIGNALS (Phase 19) --- */

UENUM(BlueprintType)
enum class EVehicleType : uint8
{
	HERO_VEHICLE_NONE = 0 UMETA(DisplayName = "None"),
	Car,
	Truck,
	Bike,
	Hover,
	Trailer,
	Mount,
	Carriage,
	Air,
	Water,
	Glider,
	VoidDrive,
	Trolley,
	Rail
};

USTRUCT(BlueprintType)
struct FHeroVehicleStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Mass = 1500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector CenterOfMassOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSupportsAntiGravity = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VoidDriveMass = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TyreBurstThreshold = 500.f;
};

USTRUCT(BlueprintType)
struct FHeroTyreStatus
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsBurst = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsRimOnly = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float FrictionMultiplier = 1.0f;
};

UENUM(BlueprintType)
enum class EHeroTrafficState : uint8
{
	HERO_TRAFFIC_NONE = 0 UMETA(DisplayName = "None"),
	TS_Green UMETA(DisplayName = "Green"),
	TS_Yellow UMETA(DisplayName = "Yellow"),
	TS_Red UMETA(DisplayName = "Red")
};

USTRUCT(BlueprintType)
struct FHeroTrafficSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Traffic")
	float GreenDuration = 30.f;

	UPROPERTY(EditAnywhere, Category = "Traffic")
	float YellowDuration = 5.f;

	UPROPERTY(EditAnywhere, Category = "Traffic")
	float RedDuration = 30.f;
};

/** --- WEATHER & ENVIRONMENT (Phase 20) --- */

UENUM(BlueprintType)
enum class EHeroOutpostState : uint8
{
	HERO_OUTPOST_NONE = 0 UMETA(DisplayName = "None"),
	Neutral,
	Friendly,
	Hostile,
	Contested,
	Captured,
	Occupied,
	Upgraded
};

USTRUCT(BlueprintType)
struct FHeroOutpostFurniture
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> FurnitureClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform Transform;
};

USTRUCT(BlueprintType)
struct FHeroOutpostFurnitureList
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FHeroOutpostFurniture> Items;
};

USTRUCT(BlueprintType)
struct FHeroGuardAssignment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EHeroNPCProfile NPCID = EHeroNPCProfile::Citizen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector PostLocation = FVector::ZeroVector;
};

USTRUCT(BlueprintType)
struct FHeroGuardAssignmentList
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FHeroGuardAssignment> Assignments;
};

UENUM(BlueprintType)
enum class EHeroWeatherType : uint8
{
	HERO_WEATHER_NONE = 0 UMETA(DisplayName = "None"),
	Clear          UMETA(DisplayName = "Clear Skies"),
	Rain           UMETA(DisplayName = "Heavy Rain"),
	Storm          UMETA(DisplayName = "Thunderstorm"),
	RadioactiveFog UMETA(DisplayName = "Radioactive Fog (Dying Light)"),
	CinematicColdOvercast UMETA(DisplayName = "Cinematic Cold Overcast (Phase 20-B)")
};

UENUM(BlueprintType)
enum class EHeroLedgeAnimId : uint8
{
	HERO_CLIMB_NONE = 0 UMETA(DisplayName = "None"),
	HERO_CLIMB_ThirdPerson_CanJumpBack,
	HERO_CLIMB_ThirdPerson_CanJumpBack_Hang,
	HERO_CLIMB_ThirdPerson_CanJumpBack_HangLoop,
	HERO_CLIMB_ThirdPerson_CanJumpLeft,
	HERO_CLIMB_ThirdPerson_CanJumpLeft_Hang,
	HERO_CLIMB_ThirdPerson_CanJumpLeft_HangLoop,
	HERO_CLIMB_ThirdPerson_CanJumpRight,
	HERO_CLIMB_ThirdPerson_CanJumpRight_Hang,
	HERO_CLIMB_ThirdPerson_CanJumpRight_HangLoop,
	HERO_CLIMB_ThirdPerson_ClimbFail,
	HERO_CLIMB_ThirdPerson_ClimbFail_Hang,
	HERO_CLIMB_ThirdPerson_ClimbUp,
	HERO_CLIMB_ThirdPerson_ClimbUp_Hang,
	HERO_CLIMB_ThirdPerson_FreeHangTransition_Left,
	HERO_CLIMB_ThirdPerson_FreeHangTransition_Right,
	HERO_CLIMB_ThirdPerson_HangTransition_Left,
	HERO_CLIMB_ThirdPerson_HangTransition_Right,
	HERO_CLIMB_ThirdPerson_Idle,
	HERO_CLIMB_ThirdPerson_Idle_Down,
	HERO_CLIMB_ThirdPerson_Idle_Up,
	HERO_CLIMB_ThirdPerson_JumpBack,
	HERO_CLIMB_ThirdPerson_JumpBack_Hang,
	HERO_CLIMB_ThirdPerson_JumpLeft,
	HERO_CLIMB_ThirdPerson_JumpLeft_Hang,
	HERO_CLIMB_ThirdPerson_JumpRight,
	HERO_CLIMB_ThirdPerson_JumpRight_Hang,
	HERO_CLIMB_ThirdPerson_JumpUp,
	HERO_CLIMB_ThirdPerson_JumpUp_Hang,
	HERO_CLIMB_ThirdPerson_LedgeHang,
	HERO_CLIMB_ThirdPerson_LedgeHang_Free,
	HERO_CLIMB_ThirdPerson_LedgeHang_Free_Loop,
	HERO_CLIMB_ThirdPerson_MoveLeft,
	HERO_CLIMB_ThirdPerson_MoveLeft_Hang,
	HERO_CLIMB_ThirdPerson_MoveRight,
	HERO_CLIMB_ThirdPerson_MoveRight_Hang,
	HERO_CLIMB_ThirdPerson_Run,
	HERO_CLIMB_ThirdPerson_Run_Down,
	HERO_CLIMB_ThirdPerson_Run_Up,
	HERO_CLIMB_ThirdPerson_TurnLeft90,
	HERO_CLIMB_ThirdPerson_TurnLeft90_Hang,
	HERO_CLIMB_ThirdPerson_TurnRight90,
	HERO_CLIMB_ThirdPerson_TurnRight90_Hang,
	HERO_CLIMB_ThirdPerson_Walk,
	HERO_CLIMB_ThirdPerson_Walk_Down,
	HERO_CLIMB_ThirdPerson_Walk_Up,
	HERO_CLIMB_ThirdPersonIdleJump_End,
	HERO_CLIMB_ThirdPersonIdleJump_Loop,
	HERO_CLIMB_ThirdPersonIdleJump_Reach_Loop,
	HERO_CLIMB_ThirdPersonIdleJump_Reach_Start,
	HERO_CLIMB_ThirdPersonIdleJump_Start,
	HERO_CLIMB_ThirdPersonRunJump_End,
	HERO_CLIMB_ThirdPersonRunJump_Loop,
	HERO_CLIMB_ThirdPersonRunJump_Reach_End,
	HERO_CLIMB_ThirdPersonRunJump_Reach_Loop,
	HERO_CLIMB_ThirdPersonRunJump_Reach_Start,
	HERO_CLIMB_ThirdPersonRunJump_Start
};
