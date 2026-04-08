// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HeroCoreTypes.h"
#include "HeroPlayerController.generated.h"


class UHeroWidget_MasterMenu;
class UHeroNavigationComponent;
class UHeroSenseComponent;

/**
 * AAA EXECUTIVE PLAYER CONTROLLER
 * Central hub for UI, Inventory Toggling, and Global Input States.
 */
UCLASS()
class HERO_API AHeroPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AHeroPlayerController();

	// ═════════════════════════════════════════════
	// 1. MASTER MENU (UI & INACTION)
	// ═════════════════════════════════════════════

	/** Toggles the Master Menu (Inventory/Skills/Quests) */
	UFUNCTION(BlueprintCallable, Category = "Hero|UI")
	void ToggleMasterMenu();

	/** Toggles between Minimap and Fullscreen 3D Live Map */
	UFUNCTION(BlueprintCallable, Category = "Hero|Map")
	void ToggleHybridMapMode();

	/** Updates the current navigation target and path */
	UFUNCTION(BlueprintCallable, Category = "Hero|Nav")
	void SetNewDestination(FVector NewLocation, EHeroTrailType Type = EHeroTrailType::TRAIL_Road);

	/** Target Widget Class (Assigned in BP_ExPlayerController) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hero|UI")
	TSubclassOf<UHeroWidget_MasterMenu> MasterMenuClass;

	// ═════════════════════════════════════════════
	// 2. MULTIPLAYER COMMANDS (Phase 60.1)
	// ═════════════════════════════════════════════

	/** Starts a Listen Server on the specified map */
	UFUNCTION(Exec, Category = "Hero|Multiplayer")
	void Host(FString MapName);

	/** Joins a server at the specified IP address */
	UFUNCTION(Exec, Category = "Hero|Multiplayer")
	void Join(FString IPAddress);

	// ═════════════════════════════════════════════
	// 3. UNIVERSAL POSSESSION SYSTEM
	// ═════════════════════════════════════════════

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hero|Possession")
	class UHeroInputComponent* HeroInput;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hero|Possession")
	class UHeroCameraManagerComponent* HeroCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hero|Possession")
	class UHeroHUDManagerComponent* HeroHUD;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hero|Dialogue")
	class UHeroDialogueComponent* HeroDialogue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hero|Nav")
	UHeroNavigationComponent* HeroNav;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hero|Sense")
	UHeroSenseComponent* HeroSense;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	// ═════════════════════════════════════════════
	// 2. NAV & MAP DATA (Checklist v11.0)
	// ═════════════════════════════════════════════

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Nav")
	FHeroNavigationData CurrentNavData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Map")
	FHero3DMapData CurrentMapData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Map")
	EHeroMapViewMode CurrentViewMode = EHeroMapViewMode::MAP_Standard;

private:
	/** Stored reference to the active menu widget */
	UPROPERTY()
	TObjectPtr<UHeroWidget_MasterMenu> MasterMenuWidget;

	bool bIsMenuOpen = false;

	/** Internal helper to restore Game-Only input */
	void CloseMenu();

	/** Internal helper to setup UI input */
	void OpenMenu();
};
