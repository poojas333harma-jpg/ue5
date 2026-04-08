// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LedgeDataTypes.h"
#include "LedgeMontageSet.generated.h"

/**
 * THE ABSOLUTE FINAL MASTER SCHEMA (PURE 100% SYNC)
 * ════════════════════════════════════════════════════
 */
UCLASS(BlueprintType)
class HERO_API ULedgeMontageSet : public UDataAsset
{
    GENERATED_BODY()

public:
	// ═════════════════════════════════════════════
	// 01 | PARKOUR - VAULTS & WINDOWS (+)
	// ═════════════════════════════════════════════
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "01 | Parkour - Vaults")
	TArray<TObjectPtr<UAnimMontage> > Vault_Safety_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "01 | Parkour - Vaults")
	TArray<TObjectPtr<UAnimMontage> > Vault_Kong_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "01 | Parkour - Vaults")
	TArray<TObjectPtr<UAnimMontage> > Vault_Speed_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "01 | Parkour - Vaults")
	TArray<TObjectPtr<UAnimMontage> > Vault_Dash_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "01 | Parkour - Vaults")
	TArray<TObjectPtr<UAnimMontage> > Vault_Lazy_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "01 | Parkour - Vaults")
	TArray<TObjectPtr<UAnimMontage> > Vault_Window_Montage;

	// ═════════════════════════════════════════════
	// 02 | PURE 42 - CORE ACTIONS (+)
	// ═════════════════════════════════════════════
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "02 | Pure 42 - Actions")
	TArray<TObjectPtr<UAnimMontage> > LedgeHang_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "02 | Pure 42 - Actions")
	TArray<TObjectPtr<UAnimMontage> > LedgeHang_Free_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "02 | Pure 42 - Actions")
	UAnimMontage* LedgeHang_Free_Loop_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "02 | Pure 42 - Actions")
	TArray<TObjectPtr<UAnimMontage> > ClimbUp_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "02 | Pure 42 - Actions")
	TArray<TObjectPtr<UAnimMontage> > ClimbUp_Hang_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "02 | Pure 42 - Actions")
	TArray<TObjectPtr<UAnimMontage> > ClimbFail_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "02 | Pure 42 - Actions")
	TArray<TObjectPtr<UAnimMontage> > ClimbFail_Hang_Montage;

	// ═════════════════════════════════════════════
	// 03 | PURE 42 - JUMPING & MOVEMENT
	// ═════════════════════════════════════════════
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "03 | Pure 42 - Jumping")
	UAnimMontage* JumpUp_Montage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "03 | Pure 42 - Jumping")
	UAnimMontage* JumpUp_Hang_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "03 | Pure 42 - Jumping")
	UAnimMontage* JumpBack_Montage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "03 | Pure 42 - Jumping")
	UAnimMontage* JumpBack_Hang_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "03 | Pure 42 - Jumping")
	UAnimMontage* JumpLeft_Montage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "03 | Pure 42 - Jumping")
	UAnimMontage* JumpLeft_Hang_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "03 | Pure 42 - Jumping")
	UAnimMontage* JumpRight_Montage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "03 | Pure 42 - Jumping")
	UAnimMontage* JumpRight_Hang_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "04 | Pure 42 - Shimmy")
	UAnimMontage* MoveLeft_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "04 | Pure 42 - Shimmy")
	UAnimMontage* MoveLeft_Hang_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "04 | Pure 42 - Shimmy")
	UAnimMontage* MoveRight_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "04 | Pure 42 - Shimmy")
	UAnimMontage* MoveRight_Hang_Montage;

    // ═════════════════════════════════════════════
	// 05 | PURE 42 - REACH & LOOPS
	// ═════════════════════════════════════════════
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "05 | Pure 42 - Reach")
	UAnimMontage* IdleJump_Start_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "05 | Pure 42 - Reach")
	UAnimMontage* IdleJump_Reach_Start_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "05 | Pure 42 - Reach")
	UAnimMontage* IdleJump_Reach_Loop_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "05 | Pure 42 - Reach")
	UAnimMontage* RunJump_Start_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "05 | Pure 42 - Reach")
	UAnimMontage* RunJump_Reach_Start_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "05 | Pure 42 - Reach")
	UAnimMontage* RunJump_Reach_Loop_Montage;

	// ═════════════════════════════════════════════
	// 06 | PURE 42 - TRANSITIONS & PREDICTIVE
	// ═════════════════════════════════════════════
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "06 | Pure 42 - Transitions")
	UAnimMontage* TurnLeft90_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "06 | Pure 42 - Transitions")
	UAnimMontage* TurnRight90_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "06 | Pure 42 - Transitions")
	UAnimMontage* HangTransition_Left_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "06 | Pure 42 - Transitions")
	UAnimMontage* HangTransition_Right_Montage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "06 | Pure 42 - Transitions")
	UAnimMontage* FreeHangTransition_Left_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "06 | Pure 42 - Transitions")
	UAnimMontage* FreeHangTransition_Right_Montage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "06 | Pure 42 - Transitions")
	UAnimMontage* TurnLeft90_Hang_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "06 | Pure 42 - Transitions")
	UAnimMontage* TurnRight90_Hang_Montage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "06 | Pure 42 - Predictive")
	UAnimMontage* CanJumpBack_Montage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "06 | Pure 42 - Predictive")
	UAnimMontage* CanJumpBack_HangLoop_Montage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "06 | Pure 42 - Predictive")
	UAnimMontage* CanJumpLeft_Montage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "06 | Pure 42 - Predictive")
	UAnimMontage* CanJumpLeft_HangLoop_Montage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "06 | Pure 42 - Predictive")
	UAnimMontage* CanJumpRight_Montage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "06 | Pure 42 - Predictive")
	UAnimMontage* CanJumpRight_HangLoop_Montage;

	// ═════════════════════════════════════════════
	// 07 | FUNCTIONAL - LADDER
	// ═════════════════════════════════════════════
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "07 | Functional - Ladder")
	UAnimMontage* Ladder_Enter_Bottom_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "07 | Functional - Ladder")
	UAnimMontage* Ladder_Enter_Top_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "07 | Functional - Ladder")
	UAnimMontage* Ladder_Exit_Bottom_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "07 | Functional - Ladder")
	UAnimMontage* Ladder_Exit_Top_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "07 | Functional - Ladder")
	TArray<TObjectPtr<UAnimMontage> > Ladder_ClimbUp_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "07 | Functional - Ladder")
	TArray<TObjectPtr<UAnimMontage> > Ladder_ClimbDown_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "07 | Functional - Ladder")
	TArray<TObjectPtr<UAnimMontage> > Ladder_Idle_Montage;

	// ═════════════════════════════════════════════
	// 08 | FUNCTIONAL - PIPES (THIN & THICK)
	// ═════════════════════════════════════════════
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "08 | Functional - Pipe (Thin)")
	UAnimMontage* Pipe_Thin_Enter_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "08 | Functional - Pipe (Thin)")
	UAnimMontage* Pipe_Thin_Exit_Montage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "08 | Functional - Pipe (Thin)")
	TArray<TObjectPtr<UAnimMontage> > Pipe_Thin_ClimbUp_Montage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "08 | Functional - Pipe (Thin)")
	TArray<TObjectPtr<UAnimMontage> > Pipe_Thin_ClimbDown_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "08 | Functional - Pipe (Thin)")
	TArray<TObjectPtr<UAnimMontage> > Pipe_Thin_MoveLeft_Montage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "08 | Functional - Pipe (Thin)")
	TArray<TObjectPtr<UAnimMontage> > Pipe_Thin_MoveRight_Montage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "08 | Functional - Pipe (Thin)")
	TArray<TObjectPtr<UAnimMontage> > Pipe_Thin_Idle_Montage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "08 | Functional - Pipe (Thick)")
	UAnimMontage* Pipe_Thick_Enter_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "08 | Functional - Pipe (Thick)")
	UAnimMontage* Pipe_Thick_Exit_Montage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "08 | Functional - Pipe (Thick)")
	TArray<TObjectPtr<UAnimMontage> > Pipe_Thick_ClimbUp_Montage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "08 | Functional - Pipe (Thick)")
	TArray<TObjectPtr<UAnimMontage> > Pipe_Thick_ClimbDown_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "08 | Functional - Pipe (Thick)")
	TArray<TObjectPtr<UAnimMontage> > Pipe_Thick_MoveLeft_Montage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "08 | Functional - Pipe (Thick)")
	TArray<TObjectPtr<UAnimMontage> > Pipe_Thick_MoveRight_Montage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "08 | Functional - Pipe (Thick)")
	TArray<TObjectPtr<UAnimMontage> > Pipe_Thick_Idle_Montage;

	// ═════════════════════════════════════════════
	// 09 | FUNCTIONAL - ZIPLINE, ROPE, BAL, SQZ
	// ═════════════════════════════════════════════
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "09 | Functional - Specialized")
	UAnimMontage* Zipline_Enter_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "09 | Functional - Specialized")
	UAnimMontage* Zipline_Exit_Montage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "09 | Functional - Specialized")
	TArray<TObjectPtr<UAnimMontage> > Zipline_Loop_Montage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "09 | Functional - Specialized")
	UAnimMontage* Rope_Enter_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "09 | Functional - Specialized")
	UAnimMontage* Rope_Exit_Montage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "09 | Functional - Specialized")
	TArray<TObjectPtr<UAnimMontage> > Rope_ClimbUp_Montage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "09 | Functional - Specialized")
	TArray<TObjectPtr<UAnimMontage> > Rope_ClimbDown_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "09 | Functional - Specialized")
	TArray<TObjectPtr<UAnimMontage> > Rope_Idle_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "09 | Functional - Specialized")
	UAnimMontage* Squeeze_Enter_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "09 | Functional - Specialized")
	UAnimMontage* Squeeze_Exit_Montage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "09 | Functional - Specialized")
	TArray<TObjectPtr<UAnimMontage> > Squeeze_Walk_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "09 | Functional - Specialized")
	UAnimMontage* Balance_Enter_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "09 | Functional - Specialized")
	UAnimMontage* Balance_Exit_Montage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "09 | Functional - Specialized")
	TArray<TObjectPtr<UAnimMontage> > Balance_Move_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "09 | Functional - Specialized")
	UAnimMontage* Balance_FallOff_Montage;

    // ═════════════════════════════════════════════
	// 10 | UNIVERSAL & LANDING
	// ═════════════════════════════════════════════
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "10 | Universal")
	TArray<TObjectPtr<UAnimMontage> > Landing_Roll_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "10 | Universal")
	TArray<TObjectPtr<UAnimMontage> > Landing_Impact_Montage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "10 | Universal")
	UAnimMontage* Traversal_JumpExit_Montage;
};