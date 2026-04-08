// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/LedgeDetectorComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SplineComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionWarpingComponent.h"
#include "Ledge/LedgeMontageSet.h"
#include "DrawDebugHelpers.h"
#include "MoverComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"
#include "UnrealClient.h"

// Log Category for Ledge Testing
DECLARE_LOG_CATEGORY_EXTERN(LogHeroLedge, Log, All);
DEFINE_LOG_CATEGORY(LogHeroLedge);

static UAnimMontage* GetRandomMontage(const TArray<TObjectPtr<UAnimMontage>>& Array, const FString& DebugName)
{
    if (Array.Num() == 0)
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("MISSING ASSET: %s"), *DebugName));
        return nullptr;
    }
    return Array[FMath::RandRange(0, Array.Num() - 1)].Get();
}

ULedgeDetectorComponent::ULedgeDetectorComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    TraceChannel = ECC_WorldStatic; // 🚩 सुबह वाला चैनल (ECC_WorldStatic)
    bShowDebugTraces = true;
}


void ULedgeDetectorComponent::BeginPlay()
{
    Super::BeginPlay();

    // FORCE AWAKE - AAA WORLD LEVEL DIAGNOSTICS
    bShowDebugTraces = true; 
    SetComponentTickEnabled(true);
    PrimaryComponentTick.bCanEverTick = true;

    // LOUD STARTUP DIAGNOSTIC
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(776, 10.f, FColor::Cyan, TEXT("--- [LEDGE SYSTEM ONLINE] ---"));
        UE_LOG(LogHeroLedge, Log, TEXT("LEDGE SYSTEM ONLINE: Initialized and Forced-Ticking for %s"), *GetOwner()->GetName());
    }
}


void ULedgeDetectorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bIsReaching) HandleMidAirReach(DeltaTime);
    if (bIsSliding) UpdateSliding(DeltaTime);
    AActor* Owner = GetOwner();

    // ★ AUTO-GRAB — ONLY when character is FALLING (airborne), not on ground ★
    UMoverComponent* Mover = Owner ? Owner->FindComponentByClass<UMoverComponent>() : nullptr;
    if (Mover && GEngine)
    {
        GEngine->AddOnScreenDebugMessage(770, 0.f, FColor::White,
            FString::Printf(TEXT("[MOVER MODE] = '%s'"), *Mover->GetMovementModeName().ToString()));
    }

    // Sirf Falling mode mein grab attempt karo — walking/ground pe NAHI
    // Also guard: only grab if falling fast enough (>100 cm/s downward) — prevents floor sticking
    bool bIsFalling = Mover && Mover->GetMovementModeName() == FName("Falling");
    float DownVelocity = 0.f;
    if (bIsFalling && Owner)
    {
        DownVelocity = -Owner->GetVelocity().Z; // Positive = falling down
    }
    if (Owner && !bIsHanging && bIsFalling && DownVelocity > -50.f) // 🚩 RELAXED: Ab jump ke apex par bhi grab karega (-50 to +infinity)
    {
        FLedgeTraceInputs DefaultInputs;
        TryGrabLedge(DefaultInputs);
    }

    // ═══ HANG POSITION LOCK — HARD SNAP EVERY FRAME (ROCK SOLID) ═══
    if (bIsHanging && bHangPositionValid && Owner)
    {
        // ● MaxHangTime fallback
        if (MaxHangTime > 0.f)
        {
            HangElapsedTime += DeltaTime;
            if (HangElapsedTime >= MaxHangTime)
            {
                UE_LOG(LogHeroLedge, Warning, TEXT("[HANG] MaxHangTime reached (%.1fs) — auto-releasing"), HangElapsedTime);
                InternalReleaseHang();
                return;
            }
        }

        // ★★★ STAMINA DRAIN — Dying Light 2 Style ★★★
        if (bStaminaDrainEnabled)
        {
            CurrentStamina -= StaminaDrainRate * DeltaTime;
            CurrentStamina = FMath::Clamp(CurrentStamina, 0.f, MaxStamina);

            float StaminaPct = (MaxStamina > 0.f) ? (CurrentStamina / MaxStamina) * 100.f : 0.f;

            // 5% — RED WARNING + Screen flash
            if (StaminaPct <= 5.f && StaminaPct > 0.f)
            {
                if (GEngine) GEngine->AddOnScreenDebugMessage(810, 0.f, FColor::Red,
                    TEXT("⚠️ STAMINA CRITICAL! HAATH CHHOOT RAHA HAI!"));
            }

            // 0% — FALL! Play ClimbFail montage
            if (CurrentStamina <= 0.f)
            {
                UE_LOG(LogHeroLedge, Warning, TEXT("[STAMINA] Stamina = 0 — ClimbFail! Gir gaya!"));
                if (GEngine) GEngine->AddOnScreenDebugMessage(810, 3.f, FColor::Red,
                    TEXT("❌ STAMINA KHATAM — GIR GAYA!"));

                // Play ClimbFail montage before releasing
                USkeletalMeshComponent* StMesh = Owner->FindComponentByClass<USkeletalMeshComponent>();
                UAnimInstance* StAnim = StMesh ? StMesh->GetAnimInstance() : nullptr;
                if (StAnim && TraversalDataAsset)
                {
                    UAnimMontage* FailMontage = GetRandomMontage(
                        (CurrentHangType == ELedgeType::Wall) ? TraversalDataAsset->ClimbFail_Montage : TraversalDataAsset->ClimbFail_Hang_Montage,
                        TEXT("ClimbFail"));
                    if (FailMontage)
                    {
                        StAnim->StopAllMontages(0.1f);
                        StAnim->Montage_Play(FailMontage, 1.0f);
                    }
                }
                InternalReleaseHang();
                return;
            }

            // Debug HUD
            if (GEngine)
            {
                StaminaPct = (CurrentStamina / MaxStamina) * 100.f;
                FColor StamColor = (StaminaPct > 30.f) ? FColor::Cyan : ((StaminaPct > 10.f) ? FColor::Yellow : FColor::Red);
                GEngine->AddOnScreenDebugMessage(811, 0.f, StamColor,
                    FString::Printf(TEXT("[STAMINA] %.0f/%.0f (%.1f%%)"), CurrentStamina, MaxStamina, StaminaPct));
            }
        }

        // ★★★ AAA STATIC & DYNAMIC SNAP — Anchor character to wall, prevent drift ★★★
        UPrimitiveComponent* RootPrim = Cast<UPrimitiveComponent>(Owner->GetRootComponent());
        if (RootPrim && !bIsPerformingAction)
        {
            // ONLY zero velocity if we are completely idle on the ledge.
            // If performing an action (Climbing/Shimmy), the animation Root Motion 
            // needs to move the character. Forcing zero causes violently shaking ("Fadfadana").
            RootPrim->SetPhysicsLinearVelocity(FVector::ZeroVector);
            RootPrim->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
            if (Mover) Mover->SetVelocity(FVector::ZeroVector);

            // ── AAA DYNAMIC HANG LOCK ──
            // IDLE HANG: "Paththar-lock" (Strict Anchor) — character stays 100% still
            float DriftDist = FVector::Dist(Owner->GetActorLocation(), CurrentHangTargetLocation);
            if (DriftDist > 5.0f) // Increased drift threshold slightly to prevent micro-fighting
            {
                Owner->SetActorLocation(CurrentHangTargetLocation, false, nullptr, ETeleportType::TeleportPhysics);
                Owner->SetActorRotation(CurrentHangTargetRotation);
            }
        }
        else if (RootPrim && bIsPerformingAction && !bIsClimbingUp)
        {
            // DYNAMIC LOCK (SHIMMY ONLY): Allow lateral movement (A/D) but lock depth/height
            // We DO NOT force zero velocity here, so animation root motion can do its job.
            FVector CurrentLoc = Owner->GetActorLocation();
            FVector WallRight = FVector::CrossProduct(FVector::UpVector, CurrentHangWallNormal).GetSafeNormal();
            
            // Project current movement onto the wall surface
            FVector ToCurrent = CurrentLoc - CurrentHangTargetLocation;
            float LateralOffset = FVector::DotProduct(ToCurrent, WallRight);

            // Safe Position = Anchor + Lateral Movement
            FVector SafeLoc = CurrentHangTargetLocation + (WallRight * LateralOffset);
            Owner->SetActorLocation(SafeLoc, false, nullptr, ETeleportType::TeleportPhysics);
            Owner->SetActorRotation(CurrentHangTargetRotation);
        }
        // IF bIsClimbingUp is TRUE, we let the character move completely freely upward!

        // ═══ DIRECT KEYBOARD INPUT — RISING EDGE + INPUT CONSUMPTION ═══
        APawn* HangPawn = Cast<APawn>(Owner);
        APlayerController* PC = HangPawn ? Cast<APlayerController>(HangPawn->GetController()) : nullptr;
        if (!PC) PC = GetWorld()->GetFirstPlayerController();
        if (PC)
        {
            // SUPREME INPUT CHECK: Poll directly from Viewport Client to bypass "Enhanced Input" consumption
            bool bW = false, bA = false, bD = false, bS = false, bC = false, bSpace = false;
            ULocalPlayer* LP = PC->GetLocalPlayer();
            if (LP && LP->ViewportClient)
            {
                FViewport* VP = LP->ViewportClient->Viewport;
                if (VP)
                {
                    bW = VP->KeyState(EKeys::W);
                    bA = VP->KeyState(EKeys::A);
                    bD = VP->KeyState(EKeys::D);
                    bS = VP->KeyState(EKeys::S);
                    bC = VP->KeyState(EKeys::C) || VP->KeyState(EKeys::LeftControl);
                    bSpace = VP->KeyState(EKeys::SpaceBar);
                }
            }

            // Fallback: raw IsInputKeyDown + Enhanced Input compatibility for W/A/S/D mapping
            // (Assumes standard enhanced input vectors are mapping to these keys if they are down)
            if (!bW) bW = PC->IsInputKeyDown(EKeys::W) || PC->IsInputKeyDown(EKeys::Gamepad_LeftY);
            if (!bA) bA = PC->IsInputKeyDown(EKeys::A) || PC->IsInputKeyDown(EKeys::Gamepad_LeftX);
            if (!bD) bD = PC->IsInputKeyDown(EKeys::D); // Gamepad X handled in A via axis mostly
            if (!bS) bS = PC->IsInputKeyDown(EKeys::S);
            if (!bC) bC = PC->IsInputKeyDown(EKeys::C) || PC->IsInputKeyDown(EKeys::LeftControl) || PC->IsInputKeyDown(EKeys::Gamepad_FaceButton_Right);
            if (!bSpace) bSpace = PC->IsInputKeyDown(EKeys::SpaceBar) || PC->IsInputKeyDown(EKeys::Gamepad_FaceButton_Bottom);

            // DEBUG INPUT ON SCREEN
            if (bShowDebugTraces && GEngine)
            {
                if (bW) GEngine->AddOnScreenDebugMessage(901, 0.1f, FColor::Orange, TEXT("SUPREME INPUT: W"));
                if (bA) GEngine->AddOnScreenDebugMessage(902, 0.1f, FColor::Orange, TEXT("SUPREME INPUT: A"));
                if (bD) GEngine->AddOnScreenDebugMessage(903, 0.1f, FColor::Orange, TEXT("SUPREME INPUT: D"));
                if (bS) GEngine->AddOnScreenDebugMessage(904, 0.1f, FColor::Orange, TEXT("SUPREME INPUT: S"));
                if (bC) GEngine->AddOnScreenDebugMessage(906, 0.1f, FColor::Orange, TEXT("SUPREME INPUT: C/CTRL"));
                if (bSpace) GEngine->AddOnScreenDebugMessage(905, 0.1f, FColor::Orange, TEXT("SUPREME INPUT: SPACE"));
            }

            // Clear Jump-W flag once W is released
            if (!bW) bWasInJumpW = false;

            if (!bIsPerformingAction && HangElapsedTime > 0.4f)
            {
                // Directional + Jump logic
                if (bSpace && !bWasSpaceDown)
                {
                    if (bA)
                    {
                        // Play Jump Left from hang
                        if (TryPlayLedgeInteraction(FLedgeTraceInputs(), TraversalDataAsset, 0.f, -1.f, true, true, CurrentHangType))
                            bIsPerformingAction = true;
                    }
                    else if (bD)
                    {
                        // Play Jump Right from hang
                        if (TryPlayLedgeInteraction(FLedgeTraceInputs(), TraversalDataAsset, 0.f, 1.f, true, true, CurrentHangType))
                            bIsPerformingAction = true;
                    }
                    else if (bS)
                    {
                        // S + Jump = Drop down (or jump backwards if implemented)
                        if (TryPlayLedgeInteraction(FLedgeTraceInputs(), TraversalDataAsset, -1.f, 0.f, true, true, CurrentHangType))
                            bIsPerformingAction = true;
                        else
                            ReleaseHang(); 
                    }
                    else
                    {
                        // W/None + Space = Climb Up
                        if (TryClimbUp()) bWasSpaceDown = bSpace;
                    }
                    bWasSpaceDown = bSpace;
                }
                // Just W = Climb Up
                else if (bW && !bWasWDown && !bWasInJumpW)
                {
                    if (TryClimbUp()) bWasWDown = bW;
                }
                // A/D = Shimmy
                else if (bA && !bWasADown)
                {
                    if (TryShimmy(-1.f)) bWasADown = bA;
                }
                else if (bD && !bWasDDown)
                {
                    if (TryShimmy(1.f)) bWasDDown = bD;
                }
                
                // C / LCTRL = Release Hang
                if (bC && !bWasCDown)
                {
                    ReleaseHang();
                    bWasCDown = bC;
                }
            }

            // Track state for rising edge
            bWasWDown = bW;
            bWasADown = bA;
            bWasDDown = bD;
            bWasSDown = bS;
            bWasCDown = bC;
            bWasSpaceDown = bSpace;
        }

        if (bShowDebugTraces && GEngine)
            GEngine->AddOnScreenDebugMessage(798, 0.f, FColor::Purple,
                FString::Printf(TEXT("[HANG LOCK] Z=%.1f T=%.1f/%.1f"),
                    Owner->GetActorLocation().Z, HangElapsedTime, MaxHangTime));
    }

    // ═══ DEBUG WALL SCAN (only when not hanging) ═══
    if (Owner && bShowDebugTraces && !bIsHanging)
    {
        FVector Origin = Owner->GetActorLocation() + FVector(0, 0, 25.f);
        FVector Forward = Owner->GetActorForwardVector();
        FVector End = Origin + (Forward * 150.f);

        FCollisionQueryParams DiagParams;
        DiagParams.AddIgnoredActor(Owner);

        FHitResult DiagHit;
        bool bDiagHit = GetWorld()->SweepSingleByChannel(
            DiagHit, Origin, End,
            FQuat::Identity,
            ECC_WorldStatic,
            FCollisionShape::MakeSphere(30.f),
            DiagParams
        );

        if (bDiagHit)
        {
            DrawDebugCapsule(GetWorld(), (Origin + End) * 0.5f, 75.f, 30.f, FRotationMatrix::MakeFromX(Forward).ToQuat(), FColor::Green, false, -1.f, 0, 2.f);
            DrawDebugSphere(GetWorld(), DiagHit.ImpactPoint, 20.f, 12, FColor::Green, false, -1.f);
            GEngine->AddOnScreenDebugMessage(780, 0.f, FColor::Green, FString::Printf(TEXT("[WALL] %s @ %.0fcm"), *DiagHit.GetActor()->GetName(), DiagHit.Distance));
        }
        else
        {
            DrawDebugCapsule(GetWorld(), (Origin + End) * 0.5f, 75.f, 30.f, FRotationMatrix::MakeFromX(Forward).ToQuat(), FColor::Red, false, -1.f, 0, 2.f);
            GEngine->AddOnScreenDebugMessage(780, 0.f, FColor::Red, TEXT("[SCANNING...]"));
        }
    }

    // (Ultra-aggressive grab is now at top of TickComponent, no velocity check needed)

    if (PredictionTimer <= 0.f) { UpdatePredictiveCheck(0.2f); PredictionTimer = PredictionInterval; }
    PredictionTimer -= DeltaTime;
}





// ═══════════════════════════════════════════════════════════════════
// THE SINGLE MAGIC NODE — TryGrabLedge
// Blueprint calls this on Jump/Interact. Everything else is automatic.
// ═══════════════════════════════════════════════════════════════════
bool ULedgeDetectorComponent::TryGrabLedge(FLedgeTraceInputs Inputs)
{
    // ── HANGING GUARD: Already hanging? Block re-trigger ──
    if (bIsHanging)
    {
        return false;
    }

    // ── COOLDOWN GUARD: Prevent spam re-grab after release ──
    float Now = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f;
    if ((Now - LastGrabTime) < 1.5f) return false;

    if (!TraversalDataAsset)
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(791, 5.f, FColor::Red, TEXT("[TryGrabLedge] ERROR: Assign TraversalDataAsset in Blueprint Details!"));
        return false;
    }

    // Universal approach — works for Mover 2.0, ACharacter, APawn, any Actor
    AActor* Owner = GetOwner();
    if (!Owner) return false;

    USkeletalMeshComponent* Mesh = Owner->FindComponentByClass<USkeletalMeshComponent>();
    if (!Mesh) return false;

    // We reached here = input/auto-grab is fully firing
    if (GEngine) GEngine->AddOnScreenDebugMessage(792, 1.f, FColor::Cyan, TEXT("[GRAB] Attempting..."));


    FLedgeCheckResult DetectResult;
    if (Inputs.TraceForwardDistance < 1.f) Inputs.TraceForwardDistance = 150.f;

    if (!TryDetectLedge(Inputs, DetectResult))
    {
        UE_LOG(LogHeroLedge, Verbose, TEXT("TryGrabLedge: TryDetectLedge returned false"));
        return false;
    }

    LastGrabTime = Now;

    // ★ INPUT CONSUMPTION — check if W is already held to prevent instant climb-up
    APlayerController* PC = Owner->GetInstigatorController<APlayerController>();
    bWasInJumpW = PC ? PC->IsInputKeyDown(EKeys::W) : false;

    if (DetectResult.ActionType == 1) // 🚩 AUTO-MANTLE: Agar height kam hai (ActionType=1), toh seedha upar chadho
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(792, 2.f, FColor::Green, TEXT("[MANTLE] Low height detected — Mantling directly!"));
        return TryMantle(TraversalDataAsset);
    }

    float H = DetectResult.ObstacleHeight;
    bool bIsTallWall = (DetectResult.ObstacleDepth >= 15.f); // 🚩 DISCRIMINATION: Wall vs Beam depth check
    UAnimMontage* MontageToPlay = bIsTallWall
        ? GetRandomMontage(TraversalDataAsset->LedgeHang_Montage, TEXT("LedgeHang"))
        : GetRandomMontage(TraversalDataAsset->LedgeHang_Free_Montage, TEXT("LedgeHang_Free"));

    // ── CAPSULE FIRST: Snap character to ledge position BEFORE playing montage ──
    FVector LedgeTop = DetectResult.FrontLedgeLocation;
    CurrentHangWallNormal = DetectResult.FrontLedgeNormal;
    CurrentHangLedgeTop = LedgeTop;

    UCapsuleComponent* Cap = Owner->FindComponentByClass<UCapsuleComponent>();
    float Radius = Cap ? Cap->GetScaledCapsuleRadius() : 35.f;
    float CapsuleHH = Cap ? Cap->GetScaledCapsuleHalfHeight() : 90.f;
    
    // AAA DYNAMIC OFFSET: Keep character safely away from wall to prevent mesh clipping (25cm)
    float WallOffset = 60.0f; // As per user requirement to prevent clipping 
    float ZOffset = -(CapsuleHH * 0.62f); 

    // FIXED: Use +CurrentHangWallNormal to move AWAY from the wall (outwards into empty space)
    CurrentHangTargetLocation = LedgeTop + FVector(CurrentHangWallNormal.X * WallOffset, CurrentHangWallNormal.Y * WallOffset, ZOffset);
    // FACING FIX: actor should FACE the wall (toward it).
    // (-WallNormal).Rotation() ALREADY points INTO the wall — do NOT add 180 again!
    CurrentHangTargetRotation = (-CurrentHangWallNormal).Rotation();
    CurrentHangTargetRotation.Yaw += MeshRotationOffset; // Only apply custom offset, not extra 180
    CurrentHangTargetRotation.Pitch = 0.f;
    CurrentHangTargetRotation.Roll = 0.f;
    bHangPositionValid = true;

    // CAMERA FIX: Disable SpringArm collision and lengthen for pull-back effect
    TArray<USpringArmComponent*> SpringArms;
    Owner->GetComponents<USpringArmComponent>(SpringArms);
    for (USpringArmComponent* Arm : SpringArms)
    {
        Arm->bDoCollisionTest = false;
        Arm->TargetArmLength += 100.f; // Extra clearance for better view
    }

    if (GEngine) GEngine->AddOnScreenDebugMessage(781, 3.f, FColor::Cyan, TEXT("[HANG] Camera Collision OFF"));

    // Visual Debug for facing
    if (bShowDebugTraces) DrawDebugDirectionalArrow(GetWorld(), CurrentHangTargetLocation, CurrentHangTargetLocation + CurrentHangTargetRotation.Vector() * 50.f, 20.f, FColor::Magenta, false, 2.0f, 0, 3.0f);

    // HARD SNAP BEFORE ANIMATION
    Owner->SetActorLocation(CurrentHangTargetLocation, false, nullptr, ETeleportType::TeleportPhysics);
    Owner->SetActorRotation(CurrentHangTargetRotation);

    // MOVER HARD-LOCK: Disable gravity to stop Falling mode from applying downward acceleration
    UMoverComponent* Mover = Owner->FindComponentByClass<UMoverComponent>();
    if (Mover)
    {
        Mover->SetGravityOverride(true, FVector::ZeroVector);
    }

    // STATE SETUP
    bIsHanging = true;
    CurrentHangType = bIsTallWall ? ELedgeType::Wall : ELedgeType::Beam;
    HangElapsedTime = 0.f;
    CurrentStamina = MaxStamina;
    if (Cap) Cap->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // ── MOTION WARPING — Set "FrontLedge" target ──
    UMotionWarpingComponent* MotionWarp = Owner->FindComponentByClass<UMotionWarpingComponent>();
    if (MotionWarp)
    {
        MotionWarp->AddOrUpdateWarpTargetFromLocationAndRotation(FName("FrontLedge"), LedgeTop, CurrentHangTargetRotation);
    }

    // ── PLAY MONTAGE (VISUAL ONLY) ──
    if (MontageToPlay)
    {
        UAnimInstance* AnimInst = Mesh->GetAnimInstance();
        if (AnimInst)
        {
            AnimInst->SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);
            float PlayResult = AnimInst->Montage_Play(MontageToPlay, 1.0f);
            
            FOnMontageEnded EndDelegate;
            EndDelegate.BindUObject(this, &ULedgeDetectorComponent::OnHangMontageEnded);
            AnimInst->Montage_SetEndDelegate(EndDelegate, MontageToPlay);
            
            UE_LOG(LogHeroLedge, Log, TEXT("TryGrabLedge: Switched to HANG mode. Snapped + Animation Play."));
        }
    }
    
    return bIsHanging;
}


// ═════════════════════════════════════════════
// Internal shared release — call from any path
void ULedgeDetectorComponent::InternalReleaseHang()
{
    if (!bIsHanging) return;

    bIsHanging = false;
    bHangPositionValid = false;
    bIsClimbingUp = false;
    bIsPerformingAction = false;
    bWasWDown = false;
    bWasADown = false;
    bWasDDown = false;
    bWasSDown = false;
    bWasCDown = false;
    HangElapsedTime = 0.f;

    // ★ Cooldown reset — release ke baad 1.5s tak re-grab block rahega
    LastGrabTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f;

    AActor* Owner = GetOwner();
    if (!Owner) return;

    // ● FIXED: NO TELEPORT ON RELEASE — Mover location must match start location for zero-jitter
    UCapsuleComponent* Cap = Owner->FindComponentByClass<UCapsuleComponent>();
    if (Cap)
    {
        Cap->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    }

    // Stop montage
    USkeletalMeshComponent* MeshComp = Owner->FindComponentByClass<USkeletalMeshComponent>();
    UAnimInstance* AnimInst = MeshComp ? MeshComp->GetAnimInstance() : nullptr;
    if (AnimInst)
    {
        AnimInst->StopAllMontages(0.25f);
        // Restore Root Motion to default (usually FromEverything)
        AnimInst->SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);
    }

    // Restore Mover — SYNC position first to prevent 'location disagrees' warning
    UMoverComponent* Mover = Owner->FindComponentByClass<UMoverComponent>();
    if (Mover)
    {
        // 🚩 AAA SYNC: Reset Mover's internal state to the current physics position
        // This stops the 'location disagrees' bounce.
        Mover->SetGravityOverride(false);
        // We removed QueueNextMode because "Falling" is handled automatically by gravity.
    }

    // CAMERA FIX: Restore SpringArm settings
    TArray<USpringArmComponent*> SpringArms;
    Owner->GetComponents<USpringArmComponent>(SpringArms);
    for (USpringArmComponent* Arm : SpringArms)
    {
        Arm->bDoCollisionTest = true;
        Arm->TargetArmLength -= 100.f;
    }

    if (GEngine) GEngine->AddOnScreenDebugMessage(799, 2.f, FColor::Cyan, TEXT("[LEDGE] Released ✔"));
    UE_LOG(LogHeroLedge, Log, TEXT("InternalReleaseHang: Released char, restored Mover simulation for gravity jump-off"));
}


// Blueprint-callable — call this from Jump/Drop input in BP
void ULedgeDetectorComponent::ReleaseHang()
{
    if (!bIsHanging)
    {
        UE_LOG(LogHeroLedge, Warning, TEXT("ReleaseHang: Not hanging — ignored"));
        return;
    }
    UE_LOG(LogHeroLedge, Log, TEXT("ReleaseHang: Player triggered release"));
    InternalReleaseHang();
}


// Fires when hang montage finishes or is interrupted
void ULedgeDetectorComponent::OnHangMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    UE_LOG(LogHeroLedge, Log, TEXT("OnHangMontageEnded: '%s' bInterrupted=%d bClimbUp=%d"),
        Montage ? *Montage->GetName() : TEXT("NULL"), bInterrupted, bIsClimbingUp);

    if (!bIsHanging) return;

    // ── CLIMB UP COMPLETED — teleport to top and release to standing
    if (bIsClimbingUp && !bInterrupted)
    {
        AActor* Owner = GetOwner();
        if (Owner)
        {
            // Teleport character ON TOP of the ledge
            UCapsuleComponent* Cap = Owner->FindComponentByClass<UCapsuleComponent>();
            float HH = Cap ? Cap->GetScaledCapsuleHalfHeight() : 90.f;
            FVector TopPos = CurrentHangLedgeTop + FVector(0, 0, HH + 5.f);
            // Push slightly forward onto the surface
            TopPos += (-CurrentHangWallNormal) * 30.f;
            Owner->SetActorLocation(TopPos, false, nullptr, ETeleportType::TeleportPhysics);
            UE_LOG(LogHeroLedge, Log, TEXT("ClimbUp complete — teleported to top Z=%.1f"), TopPos.Z);
        }
        InternalReleaseHang();
        return;
    }

    // ── Player ne release kiya (interrupted) → turant release karo
    if (bInterrupted)
    {
        UE_LOG(LogHeroLedge, Log, TEXT("OnHangMontageEnded: Interrupted — already releasing"));
        return;
    }

    // ── Montage naturally khatam hui → LOOP montage pe transition karo
    AActor* Owner = GetOwner();
    USkeletalMeshComponent* Mesh = Owner ? Owner->FindComponentByClass<USkeletalMeshComponent>() : nullptr;
    UAnimInstance* AnimInst = Mesh ? Mesh->GetAnimInstance() : nullptr;

    UAnimMontage* LoopMontage = TraversalDataAsset ? TraversalDataAsset->LedgeHang_Free_Loop_Montage : nullptr;

    if (AnimInst && LoopMontage)
    {
        float LoopResult = AnimInst->Montage_Play(LoopMontage, 1.0f);
        UE_LOG(LogHeroLedge, Log, TEXT("OnHangMontageEnded: Transitioned to Loop='%s' result=%.2f"),
            *LoopMontage->GetName(), LoopResult);

        if (LoopResult > 0.f)
        {
            FOnMontageEnded EndDelegate;
            EndDelegate.BindUObject(this, &ULedgeDetectorComponent::OnHangMontageEnded);
            AnimInst->Montage_SetEndDelegate(EndDelegate, LoopMontage);
        }
    }
    else
    {
        UE_LOG(LogHeroLedge, Log, TEXT("OnHangMontageEnded: No loop montage — holding hang via position lock"));
    }
}


FLedgeExecutionResult ULedgeDetectorComponent::ExecuteLedgeLogic(FLedgeTraceInputs Inputs, ULedgeMontageSet* MontageSet)
{
    FLedgeExecutionResult Result;
    if (!MontageSet) 
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ERROR: No MontageSet selected in Blueprint!"));
        return Result;
    }

    FLedgeCheckResult DetectData;
    if (!TryDetectLedge(Inputs, DetectData)) return Result;

    Result.bSuccess = true;
    Result.LedgeType = (DetectData.ObstacleDepth < 15.f && DetectData.ObstacleDepth > 0.01f) ? ELedgeType::Beam : ELedgeType::Wall;
    Result.ActionType = static_cast<ELedgeAction>(DetectData.ActionType);
    Result.WarpTransform = FTransform((-DetectData.FrontLedgeNormal).Rotation(), DetectData.FrontLedgeLocation);

    AActor* ActorOwner = GetOwner();
    UPrimitiveComponent* HitComp = DetectData.HitComponent.Get();
    float Speed = ActorOwner ? ActorOwner->GetVelocity().Size() : 0.f;

    // 01 | TAG DETECTION
    if (HitComp)
    {
        if (HitComp->ComponentHasTag(FName("Window")) && MontageSet->Vault_Window_Montage.Num() > 0)
        {
            Result.ActionType = ELedgeAction::Vault;
            Result.MontageToPlay = GetRandomMontage(MontageSet->Vault_Window_Montage, "Window");
        }
        else if (HitComp->ComponentHasTag(FName("Pipe_Thin")))
        {
            Result.LedgeType = ELedgeType::Pipe_Thin;
            Result.MontageToPlay = MontageSet->Pipe_Thin_Enter_Montage;
            if (!Result.MontageToPlay) Result.MontageToPlay = GetRandomMontage(MontageSet->Pipe_Thin_Idle_Montage, "PipeThin");
        }
    }

    // 02 | PARKOUR VAULT FALLBACK
    if (!Result.MontageToPlay && Result.ActionType == ELedgeAction::Vault)
    {
        if (Speed > 500.f) Result.MontageToPlay = GetRandomMontage(MontageSet->Vault_Kong_Montage, "Kong");
        else if (Speed > 300.f) Result.MontageToPlay = GetRandomMontage(MontageSet->Vault_Safety_Montage, "Safety");
        else Result.MontageToPlay = GetRandomMontage(MontageSet->Vault_Lazy_Montage, "Lazy");
    }

    // 03 | HANG FALLBACK
    if (!Result.MontageToPlay)
    {
        bool bIsWall = (Result.LedgeType == ELedgeType::Wall);
        if (Result.ActionType == ELedgeAction::Grab_Idle)
            Result.MontageToPlay = bIsWall ? GetRandomMontage(MontageSet->LedgeHang_Montage, "Hang") : GetRandomMontage(MontageSet->LedgeHang_Free_Montage, "HangFree");
        else
            Result.MontageToPlay = bIsWall ? GetRandomMontage(MontageSet->ClimbUp_Montage, "Climb") : GetRandomMontage(MontageSet->ClimbUp_Hang_Montage, "ClimbHang");
    }

    if (!Result.MontageToPlay) Result.MontageToPlay = MontageSet->IdleJump_Start_Montage;

    return Result;
}

bool ULedgeDetectorComponent::TryDetectLedge(const FLedgeTraceInputs& Inputs, FLedgeCheckResult& OutResult)
{
    // Universal: works for ACharacter, APawn, Mover 2.0, any Actor
    AActor* Owner = GetOwner();
    if (!Owner) return false;

    float TraceDist = (Inputs.TraceForwardDistance > 1.f) ? Inputs.TraceForwardDistance : 150.f;
    float Radius = (Inputs.TraceRadius > 1.f) ? Inputs.TraceRadius : 35.f;
    float MaxH = (Inputs.TraceMaxGrabHeight > 1.f) ? Inputs.TraceMaxGrabHeight : 250.f;

    FVector Forward = (Inputs.TraceForwardDirection.IsNearlyZero()) ? Owner->GetActorForwardVector() : Inputs.TraceForwardDirection;
    FVector Origin = Owner->GetActorLocation() + (FVector::UpVector * 25.f) + Inputs.TraceOriginOffset;


    FCollisionQueryParams FallbackParams; FallbackParams.AddIgnoredActor(Owner);

    FHitResult WallHit;
    bool bHit = TraceForwardWall(Origin, Forward, TraceDist, Radius, WallHit);

    // Fallback: WorldStatic
    if (!bHit)
    {
        bHit = GetWorld()->SweepSingleByChannel(WallHit, Origin, Origin + (Forward * TraceDist), FQuat::Identity, ECC_WorldStatic, FCollisionShape::MakeSphere(Radius), FallbackParams);
    }

    if (bHit)
    {
        if (bShowDebugTraces) DrawDebugSphere(GetWorld(), WallHit.ImpactPoint, 15.f, 12, FColor::Red, false, 0.1f);
        if (GEngine) GEngine->AddOnScreenDebugMessage(782, 0.1f, FColor::Orange, 
            FString::Printf(TEXT("[STEP1] Wall Hit: %s"), *WallHit.GetActor()->GetName()));

        OutResult.HasFrontLedge = true;
        OutResult.FrontLedgeNormal = WallHit.ImpactNormal;
        OutResult.HitComponent = WallHit.Component.Get();

        FHitResult TopHit;
        if (TraceWallTop(WallHit.ImpactPoint, Forward, MaxH, TopHit))
        {
            if (bShowDebugTraces) DrawDebugLine(GetWorld(), TopHit.TraceStart, TopHit.ImpactPoint, FColor::Yellow, false, 0.1f, 0, 2.0f);

            OutResult.FrontLedgeLocation = TopHit.ImpactPoint;
            float H = OutResult.FrontLedgeLocation.Z - Owner->GetActorLocation().Z;
            OutResult.ObstacleHeight = H;

            // 🚩 AAA DISCRIMINATION: Check depth by tracing slightly further behind impact
            FHitResult DepthHit;
            FVector DepthStart = OutResult.FrontLedgeLocation + (Forward * 20.f) + (FVector::UpVector * 10.f);
            FVector DepthEnd = DepthStart - (FVector::UpVector * 50.f);
            bool bHasDepth = GetWorld()->LineTraceSingleByChannel(DepthHit, DepthStart, DepthEnd, ECC_WorldStatic, FallbackParams);
            if (!bHasDepth) bHasDepth = GetWorld()->LineTraceSingleByChannel(DepthHit, DepthStart, DepthEnd, ECC_Visibility, FallbackParams);
            OutResult.ObstacleDepth = bHasDepth ? 50.f : 5.f;

            // Decision: Mantle vs Hang
            // Below VaultHeightThreshold (e.g. 110cm) -> Mantle (Action 1)
            // Above VaultHeightThreshold -> Hang (Action 2)
            VaultHeightThreshold = 150.f; // As per user requirement, Auto-Mantle below 1.5m
            OutResult.ActionType = (H > VaultHeightThreshold) ? 2 : 1;

            if (GEngine) GEngine->AddOnScreenDebugMessage(783, 0.1f, FColor::Yellow,
                FString::Printf(TEXT("[STEP2] Top Found! H=%.0f D=%.0f Type=%s"), H, OutResult.ObstacleDepth, (OutResult.ActionType == 1 ? TEXT("MANTLE") : TEXT("HANG"))));

            bool bResult = (H > MinGrabHeight && H < MaxH);
            if (!bResult && GEngine) GEngine->AddOnScreenDebugMessage(784, 2.f, FColor::Red,
                FString::Printf(TEXT("[FAIL] Height %.0f outside range [%.0f - %.0f]"), H, MinGrabHeight, MaxH));
            return bResult;
        }
        else
        {
            if (GEngine) GEngine->AddOnScreenDebugMessage(783, 0.1f, FColor::Red, TEXT("[STEP2] Top NOT found! Wall too tall or no top surface."));
        }
    }
    else
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(782, 0.1f, FColor::Silver, TEXT("[STEP1] No Wall Hit"));
    }
    return false;
}



bool ULedgeDetectorComponent::TraceForwardWall(const FVector& Origin, const FVector& Forward, float Distance, float Radius, FHitResult& OutHit) const
{
    FVector End = Origin + (Forward * Distance);
    FCollisionQueryParams Params; Params.AddIgnoredActor(GetOwner());
    
    // Using ECC_WorldStatic for max reliability on environment architecture
    bool bHit = GetWorld()->SweepSingleByChannel(OutHit, Origin, End, FQuat::Identity, ECC_WorldStatic, FCollisionShape::MakeSphere(Radius), Params);
    if (!bHit) bHit = GetWorld()->SweepSingleByChannel(OutHit, Origin, End, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(Radius), Params);
    
    if (bShowDebugTraces)
    {
        // 🚩 RED/GREEN SPHERES - Just like this morning
        FColor DebugColor = bHit ? FColor::Green : FColor::Red;
        DrawDebugCapsule(GetWorld(), Origin + (Forward * Distance * 0.5f), Distance * 0.5f, Radius, FRotationMatrix::MakeFromZ(Forward).ToQuat(), DebugColor, false, 0.1f);
        if (bHit) DrawDebugSphere(GetWorld(), OutHit.ImpactPoint, Radius, 8, FColor::Green, false, 0.1f);
    }
    return bHit;
}


bool ULedgeDetectorComponent::TraceWallTop(const FVector& WallHitLocation, const FVector& Forward, float MaxHeight, FHitResult& OutLedgeHit) const
{
    FCollisionQueryParams Params; Params.AddIgnoredActor(GetOwner());

    // Try multiple forward offsets and widths to catch thin beams and deep walls
    static const float ForwardOffsets[] = { 10.f, 25.f, 40.f, 60.f };
    for (float Offset : ForwardOffsets)
    {
        FVector Start = WallHitLocation + (Forward * Offset) + (FVector::UpVector * MaxHeight);
        FVector End   = Start - (FVector::UpVector * MaxHeight * 1.5f);

        // 🚩 MULTI-CHANNEL SCAN: Static -> Visibility -> WorldDynamic
        bool bHit = GetWorld()->LineTraceSingleByChannel(OutLedgeHit, Start, End, ECC_WorldStatic, Params);
        if (!bHit) bHit = GetWorld()->LineTraceSingleByChannel(OutLedgeHit, Start, End, ECC_Visibility, Params);
        
        if (bShowDebugTraces) DrawDebugLine(GetWorld(), Start, End, bHit ? FColor::Blue : FColor::Orange, false, 0.2f, 0, 3.0f);
        if (bHit) return true;
    }

    return false;
}


bool ULedgeDetectorComponent::TryPlayLedgeInteraction(FLedgeTraceInputs Inputs, ULedgeMontageSet* MontageSet, float InputForward, float InputRight, bool bJumpPressed, bool bCharacterIsHanging, ELedgeType CurrentLedgeType)
{
    if (!MontageSet) return false;
    AActor* CharOwner = GetOwner(); if (!CharOwner) return false;
    USkeletalMeshComponent* CharMesh = CharOwner->FindComponentByClass<USkeletalMeshComponent>();
    UAnimInstance* CharInst = CharMesh ? CharMesh->GetAnimInstance() : nullptr;
    if (!CharInst) return false;

    UAnimMontage* MontageToPlay = nullptr;
    bool bIsWall = (CurrentLedgeType == ELedgeType::Wall);

    if (bCharacterIsHanging)
    {
        if (bJumpPressed && InputForward < -0.5f) MontageToPlay = MontageSet->Traversal_JumpExit_Montage;
        else if (InputRight < -0.5f) MontageToPlay = bIsWall ? MontageSet->MoveLeft_Montage : MontageSet->MoveLeft_Hang_Montage;
        else if (InputRight > 0.5f) MontageToPlay = bIsWall ? MontageSet->MoveRight_Montage : MontageSet->MoveRight_Hang_Montage;
        else if (InputForward > 0.5f) MontageToPlay = bIsWall ? GetRandomMontage(MontageSet->ClimbUp_Montage, "Climb") : GetRandomMontage(MontageSet->ClimbUp_Hang_Montage, "ClimbHang");
    }

    if (MontageToPlay) { CharInst->Montage_Play(MontageToPlay); return true; }
    return false;
}


bool ULedgeDetectorComponent::JumpEject()
{
    AActor* JumpOwner = GetOwner(); if (!JumpOwner) return false;
    ACharacter* AsChar = Cast<ACharacter>(JumpOwner);
    if (AsChar) { AsChar->LaunchCharacter(FVector(0, 0, 600.f), true, true); return true; }
    return false;
}

void ULedgeDetectorComponent::UpdateStamina(float DeltaTime)
{
    CurrentStamina -= StaminaDrainRate * DeltaTime;
    if (CurrentStamina <= 0.f && TraversalDataAsset)
    {
        AActor* StOwner = GetOwner();
        USkeletalMeshComponent* StMesh = StOwner ? StOwner->FindComponentByClass<USkeletalMeshComponent>() : nullptr;
        UAnimInstance* StInst = StMesh ? StMesh->GetAnimInstance() : nullptr;
        if (StInst) StInst->Montage_Play(GetRandomMontage(TraversalDataAsset->ClimbFail_Montage, "ClimbFail"));
    }
}

bool ULedgeDetectorComponent::CheckFallImpact(float FallVelocityZ, ULedgeMontageSet* TraversalSet)
{
    if (!TraversalSet) return false;
    AActor* FallOwner = GetOwner();
    USkeletalMeshComponent* FallMesh = FallOwner ? FallOwner->FindComponentByClass<USkeletalMeshComponent>() : nullptr;
    UAnimInstance* FallInst = FallMesh ? FallMesh->GetAnimInstance() : nullptr;
    if (FallInst && FallVelocityZ < RollVelocityThreshold)
    {
        UAnimMontage* M = (FallVelocityZ < StaggerVelocityThreshold) ? GetRandomMontage(TraversalSet->Landing_Impact_Montage, "Impact") : GetRandomMontage(TraversalSet->Landing_Roll_Montage, "Roll");
        if (M) { FallInst->Montage_Play(M); return true; }
    }
    return false;
}

void ULedgeDetectorComponent::UpdateBalance(float DeltaTime, float MoveInputX)
{
    CurrentStability -= StabilityDrainRate * DeltaTime;
    if (CurrentStability <= 0.f && TraversalDataAsset)
    {
        AActor* BalOwner = GetOwner();
        USkeletalMeshComponent* BalMesh = BalOwner ? BalOwner->FindComponentByClass<USkeletalMeshComponent>() : nullptr;
        UAnimInstance* BalInst = BalMesh ? BalMesh->GetAnimInstance() : nullptr;
        if (BalInst) BalInst->Montage_Play(TraversalDataAsset->Balance_FallOff_Montage);
    }
}

bool ULedgeDetectorComponent::TryStartSlide()
{
    AActor* SlideOwner = GetOwner();
    if (SlideOwner && SlideOwner->GetVelocity().Size() > 450.f)
    {
        bIsSliding = true;
        if (TraversalDataAsset)
        {
            USkeletalMeshComponent* SM = SlideOwner->FindComponentByClass<USkeletalMeshComponent>();
            UAnimInstance* AI = SM ? SM->GetAnimInstance() : nullptr;
            if (AI) AI->Montage_Play(GetRandomMontage(TraversalDataAsset->Landing_Roll_Montage, "Slide"));
        }
        return true;
    }
    return false;
}

void ULedgeDetectorComponent::UpdateSliding(float DeltaTime)
{
    AActor* SlidOwner = GetOwner();
    if (SlidOwner && SlidOwner->GetVelocity().Size() < SlideEndSpeedThreshold) bIsSliding = false;
}

bool ULedgeDetectorComponent::TryMantle(ULedgeMontageSet* MontageSet)
{
    FLedgeTraceInputs Inputs; Inputs.TraceMaxGrabHeight = 150.f; // Match VaultHeightThreshold
    FLedgeCheckResult Hit;
    if (TryDetectLedge(Inputs, Hit) && Hit.ActionType == 1)
    {
        AActor* MantOwner = GetOwner();
        USkeletalMeshComponent* SM = MantOwner ? MantOwner->FindComponentByClass<USkeletalMeshComponent>() : nullptr;
        UAnimInstance* AI = SM ? SM->GetAnimInstance() : nullptr;
        if (AI) { AI->Montage_Play(GetRandomMontage(MontageSet->Vault_Safety_Montage, "Mantle")); return true; }
    }
    return false;
}

void ULedgeDetectorComponent::StartSqueeze(USplineComponent* TargetSpline) { ActiveSqueezeSpline = TargetSpline; bIsSqueezing = true; }
void ULedgeDetectorComponent::UpdateSqueeze(float DeltaTime, float MoveInput) {}
void ULedgeDetectorComponent::UpdatePredictiveCheck(float DeltaTime) {}
void ULedgeDetectorComponent::HandleMidAirReach(float DeltaTime) {
    AActor* MidOwner = GetOwner();
    if (MidOwner && TraversalDataAsset) {
        FLedgeTraceInputs Inputs; Inputs.TraceForwardDistance = 150.f;
        FLedgeCheckResult Hit;
        if (TryDetectLedge(Inputs, Hit)) {
            USkeletalMeshComponent* SM = MidOwner->FindComponentByClass<USkeletalMeshComponent>();
            UAnimInstance* AI = SM ? SM->GetAnimInstance() : nullptr;
            if (AI) AI->Montage_Play(TraversalDataAsset->RunJump_Reach_Start_Montage);
        }
    }
}
bool ULedgeDetectorComponent::CheckGroundStability(FName BeamTag) { return false; }


// ═══════════════════════════════════════════════════════════════════
// CLIMB UP — W (Forward) while hanging → climb to top of ledge
// ═══════════════════════════════════════════════════════════════════
bool ULedgeDetectorComponent::TryClimbUp()
{
    if (!bIsHanging || bIsPerformingAction || !TraversalDataAsset) return false;

    AActor* Owner = GetOwner();
    if (!Owner) return false;

    USkeletalMeshComponent* Mesh = Owner->FindComponentByClass<USkeletalMeshComponent>();
    UAnimInstance* AnimInst = Mesh ? Mesh->GetAnimInstance() : nullptr;
    if (!AnimInst) return false;

    // Check if there's space on top of the ledge (back clearance)
    UCapsuleComponent* Cap = Owner->FindComponentByClass<UCapsuleComponent>();
    float HH = Cap ? Cap->GetScaledCapsuleHalfHeight() : 90.f;
    float Rad = Cap ? Cap->GetScaledCapsuleRadius() : 35.f;

    // Ceiling check — sweep AWAY from wall so we don't clip into wall geometry above
    FVector CeilingCheckOffset = CurrentHangWallNormal * (Rad + 20.f); // Push away from wall
    FVector TopCheckStart = CurrentHangLedgeTop + CeilingCheckOffset + FVector(0, 0, HH * 2.f + 10.f);
    FVector TopCheckEnd = CurrentHangLedgeTop + CeilingCheckOffset + FVector(0, 0, HH + 5.f);
    FCollisionQueryParams TopParams; TopParams.AddIgnoredActor(Owner);
    FHitResult TopHit;
    bool bCeilingBlocked = GetWorld()->SweepSingleByChannel(
        TopHit, TopCheckStart, TopCheckEnd, FQuat::Identity,
        ECC_WorldStatic, FCollisionShape::MakeSphere(Rad * 0.3f), TopParams
    );

    if (bCeilingBlocked)
    {
        UE_LOG(LogHeroLedge, Log, TEXT("TryClimbUp: Ceiling blocked — cannot climb"));
        if (GEngine) GEngine->AddOnScreenDebugMessage(800, 2.f, FColor::Red, TEXT("[CLIMB] Ceiling blocked!"));
        return false;
    }

    // Pick ClimbUp montage
    bool bIsWall = (CurrentHangType == ELedgeType::Wall);
    UAnimMontage* ClimbMontage = bIsWall
        ? GetRandomMontage(TraversalDataAsset->ClimbUp_Montage, TEXT("ClimbUp"))
        : GetRandomMontage(TraversalDataAsset->ClimbUp_Hang_Montage, TEXT("ClimbUp_Hang"));

    if (!ClimbMontage)
    {
        // No montage — instant teleport to top
        FVector TopPos = CurrentHangLedgeTop + FVector(0, 0, HH + 5.f) + (-CurrentHangWallNormal) * 30.f;
        Owner->SetActorLocation(TopPos, false, nullptr, ETeleportType::TeleportPhysics);
        InternalReleaseHang();
        UE_LOG(LogHeroLedge, Log, TEXT("TryClimbUp: No montage — instant teleport to Z=%.1f"), TopPos.Z);
        return true;
    }

    // Set flag so OnHangMontageEnded knows to teleport to top
    bIsClimbingUp = true;
    bIsPerformingAction = true;

    // Enable root motion for climb animation — it needs to move the mesh visually
    AnimInst->SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);

    // Stop current loop montage and play climb
    AnimInst->StopAllMontages(0.15f);
    float PlayResult = AnimInst->Montage_Play(ClimbMontage, 1.0f);

    UE_LOG(LogHeroLedge, Log, TEXT("TryClimbUp: Playing '%s' result=%.2f"), *ClimbMontage->GetName(), PlayResult);
    if (GEngine) GEngine->AddOnScreenDebugMessage(800, 3.f, FColor::Green,
        FString::Printf(TEXT("[CLIMB UP] %s"), *ClimbMontage->GetName()));

    if (PlayResult > 0.f)
    {
        FOnMontageEnded EndDelegate;
        EndDelegate.BindUObject(this, &ULedgeDetectorComponent::OnHangMontageEnded);
        AnimInst->Montage_SetEndDelegate(EndDelegate, ClimbMontage);
    }

    return PlayResult > 0.f;
}


// ═══════════════════════════════════════════════════════════════════
// SHIMMY — A/D while hanging → slide left/right along wall edge
// ═══════════════════════════════════════════════════════════════════
bool ULedgeDetectorComponent::TryShimmy(float Direction)
{
    if (!bIsHanging || bIsPerformingAction || !TraversalDataAsset) return false;
    if (FMath::Abs(Direction) < 0.3f) return false; // Deadzone

    AActor* Owner = GetOwner();
    if (!Owner) return false;

    USkeletalMeshComponent* Mesh = Owner->FindComponentByClass<USkeletalMeshComponent>();
    UAnimInstance* AnimInst = Mesh ? Mesh->GetAnimInstance() : nullptr;

    // ── Calculate lateral direction ──
    // WallRight = Cross(Up, WallNormal) gives right when LOOKING AT wall from outside.
    // But character FACES the wall, so their right = -WallRight.
    // Fix: negate direction so D=character-right and A=character-left.
    FVector WallRight = FVector::CrossProduct(FVector::UpVector, CurrentHangWallNormal).GetSafeNormal();
    FVector MoveDir = (Direction > 0.f) ? -WallRight : WallRight; // FIXED: negate to match character POV
    bool bGoingRight = (Direction > 0.f);

    // ── STEP 1: Check if wall continues in shimmy direction ──
    FVector ShimmyTarget = Owner->GetActorLocation() + (MoveDir * ShimmyStepDistance);
    FVector WallCheckOrigin = ShimmyTarget + (CurrentHangWallNormal * 50.f); // Start from in front of wall
    FVector WallCheckEnd = ShimmyTarget - (CurrentHangWallNormal * 50.f);     // Trace into wall

    FCollisionQueryParams WallParams; WallParams.AddIgnoredActor(Owner);
    FHitResult WallCheckHit;
    bool bWallContinues = GetWorld()->LineTraceSingleByChannel(
        WallCheckHit, WallCheckOrigin, WallCheckEnd, ECC_WorldStatic, WallParams
    );

    if (bShowDebugTraces)
    {
        DrawDebugLine(GetWorld(), WallCheckOrigin, WallCheckEnd,
            bWallContinues ? FColor::Green : FColor::Red, false, 1.f, 0, 2.f);
    }

    if (!bWallContinues)
    {
        // 🚩 OUTER CORNER: Wall ends — try to wrap around!
        if (GEngine) GEngine->AddOnScreenDebugMessage(801, 1.f, FColor::Magenta, TEXT("[SHIMMY] Outer Corner! Wrapping..."));
        return TryCornerWrap(Direction);
    }

    // ── STEP 1B: Check for Inner Corner (blocking wall in front of shimmy) ──
    FVector InnerCheckOrigin = Owner->GetActorLocation() + (-CurrentHangWallNormal * 15.f);
    FVector InnerCheckEnd = InnerCheckOrigin + (MoveDir * 45.f);
    FHitResult InnerHit;
    if (GetWorld()->LineTraceSingleByChannel(InnerHit, InnerCheckOrigin, InnerCheckEnd, ECC_WorldStatic, WallParams))
    {
        // 🚩 INNER CORNER: Blocking wall hit!
        if (GEngine) GEngine->AddOnScreenDebugMessage(801, 1.f, FColor::Cyan, TEXT("[SHIMMY] Inner Corner! Wrapping..."));
        
        // Update state to the new wall
        CurrentHangWallNormal = InnerHit.ImpactNormal;
        CurrentHangTargetRotation = (-CurrentHangWallNormal).Rotation();
        CurrentHangTargetRotation.Pitch = 0.f;
        CurrentHangTargetRotation.Roll = 0.f;

        // 🚩 AAA SYNC: Trace the new ledge top!
        FHitResult NewLedgeHit;
        FVector NewForward = -CurrentHangWallNormal;
        if (TraceWallTop(InnerHit.ImpactPoint, NewForward, MaxGrabHeight, NewLedgeHit))
        {
            UCapsuleComponent* Cap = Owner->FindComponentByClass<UCapsuleComponent>();
            float CapsuleHH = Cap ? Cap->GetScaledCapsuleHalfHeight() : 90.f;
            float ZOffset = -(CapsuleHH * 0.6f);
            
            CurrentHangLedgeTop = NewLedgeHit.ImpactPoint;
            float WallOffset = (Cap ? Cap->GetScaledCapsuleRadius() : 35.f) + 15.f;
            CurrentHangTargetLocation = CurrentHangLedgeTop + (CurrentHangWallNormal * WallOffset) + (FVector::UpVector * ZOffset);
            
            Owner->SetActorLocation(CurrentHangTargetLocation, false, nullptr, ETeleportType::TeleportPhysics);
            Owner->SetActorRotation(CurrentHangTargetRotation);
        }
        
        // Play Turn90 (Internal)
        UAnimMontage* InnerTurn = bGoingRight ? TraversalDataAsset->TurnLeft90_Hang_Montage : TraversalDataAsset->TurnRight90_Hang_Montage;
        if (InnerTurn && AnimInst) {
            AnimInst->StopAllMontages(0.1f);
            AnimInst->Montage_Play(InnerTurn);
        }
        return true;
    }

    // ── STEP 2: Check ledge top at new position ──
    FHitResult LedgeTopHit;
    FVector LedgeCheckStart = ShimmyTarget + (FVector::UpVector * 200.f) + (-CurrentHangWallNormal * 25.f);
    FVector LedgeCheckEnd = LedgeCheckStart - (FVector::UpVector * 400.f);

    bool bLedgeExists = GetWorld()->LineTraceSingleByChannel(
        LedgeTopHit, LedgeCheckStart, LedgeCheckEnd, ECC_WorldStatic, WallParams
    );

    if (!bLedgeExists)
    {
        UE_LOG(LogHeroLedge, Log, TEXT("TryShimmy: No ledge at shimmy target"));
        if (GEngine) GEngine->AddOnScreenDebugMessage(801, 1.f, FColor::Red, TEXT("[SHIMMY] Edge of ledge!"));
        return false;
    }

    // ── STEP 3: Update hang position ──
    UCapsuleComponent* Cap = Owner->FindComponentByClass<UCapsuleComponent>();
    float CapsuleHH = Cap ? Cap->GetScaledCapsuleHalfHeight() : 90.f;
    float ZOffset = -(CapsuleHH * 0.6f);

    // FIXED: Use +CurrentHangWallNormal to move AWAY from the wall
    float WallOffset = (Cap ? Cap->GetScaledCapsuleRadius() : 35.f) + 25.f;
    FVector HangOffset = FVector(
        CurrentHangWallNormal.X * WallOffset,
        CurrentHangWallNormal.Y * WallOffset,
        ZOffset
    );
    CurrentHangTargetLocation = CurrentHangLedgeTop + HangOffset;
    // Rotation stays the same since wall normal hasn't changed

    // ── STEP 4: Play shimmy montage ──
    if (!AnimInst) return false;

    // ROOT MOTION OFF during shimmy — Tick anchor lock handles position, not anim root motion
    AnimInst->SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);

    bool bIsWall = (CurrentHangType == ELedgeType::Wall);
    UAnimMontage* ShimmyMontage = nullptr;

    if (bGoingRight)
        ShimmyMontage = bIsWall ? TraversalDataAsset->MoveRight_Montage : TraversalDataAsset->MoveRight_Hang_Montage;
    else
        ShimmyMontage = bIsWall ? TraversalDataAsset->MoveLeft_Montage : TraversalDataAsset->MoveLeft_Hang_Montage;

    bIsPerformingAction = true;

    if (ShimmyMontage)
    {
        AnimInst->StopAllMontages(0.1f);
        float PlayResult = AnimInst->Montage_Play(ShimmyMontage, 1.0f);
        UE_LOG(LogHeroLedge, Log, TEXT("TryShimmy: %s '%s' result=%.2f"),
            bGoingRight ? TEXT("RIGHT") : TEXT("LEFT"), *ShimmyMontage->GetName(), PlayResult);

        if (PlayResult > 0.f)
        {
            FOnMontageEnded EndDelegate;
            EndDelegate.BindUObject(this, &ULedgeDetectorComponent::OnActionMontageEnded);
            AnimInst->Montage_SetEndDelegate(EndDelegate, ShimmyMontage);
        }
    }
    else
    {
        // No montage — just move position instantly
        bIsPerformingAction = false;
        UE_LOG(LogHeroLedge, Log, TEXT("TryShimmy: No montage — instant move"));
    }

    // Snap to new position (Tick lock will smooth it)
    Owner->SetActorLocation(CurrentHangTargetLocation, false, nullptr, ETeleportType::TeleportPhysics);

    if (GEngine) GEngine->AddOnScreenDebugMessage(801, 1.f, FColor::Cyan,
        FString::Printf(TEXT("[SHIMMY] %s %.0fcm"), bGoingRight ? TEXT("→") : TEXT("←"), ShimmyStepDistance));

    return true;
}


// ═══════════════════════════════════════════════════════════════════
// CORNER WRAP — wall turns 90°, character wraps around
// ═══════════════════════════════════════════════════════════════════
bool ULedgeDetectorComponent::TryCornerWrap(float Direction)
{
    if (!bIsHanging || !TraversalDataAsset) return false;

    AActor* Owner = GetOwner();
    if (!Owner) return false;

    bool bGoingRight = (Direction > 0.f);

    // Calculate lateral direction
    FVector WallRight = FVector::CrossProduct(FVector::UpVector, CurrentHangWallNormal).GetSafeNormal();
    FVector LateralDir = bGoingRight ? WallRight : -WallRight;

    // ── Find the corner: trace sideways to find where wall ends ──
    FVector CornerSearchStart = Owner->GetActorLocation() - (CurrentHangWallNormal * 10.f); // Near wall
    FVector CornerSearchEnd = CornerSearchStart + (LateralDir * 150.f);

    FCollisionQueryParams Params; Params.AddIgnoredActor(Owner);
    FHitResult CornerHit;
    bool bFoundEdge = !GetWorld()->LineTraceSingleByChannel(
        CornerHit, CornerSearchStart, CornerSearchEnd, ECC_WorldStatic, Params
    );

    if (!bFoundEdge)
    {
        UE_LOG(LogHeroLedge, Log, TEXT("TryCornerWrap: Wall continues — no corner found"));
        return false;
    }

    // ── Trace AROUND the corner to find the next wall face ──
    // Go past the edge, then trace INTO where the next wall should be
    FVector PastEdge = CornerSearchEnd + (LateralDir * 30.f); // 30cm past the edge
    FVector IntoNextWall = PastEdge - (LateralDir.RotateAngleAxis(bGoingRight ? 90.f : -90.f, FVector::UpVector) * 100.f);

    // Actually, the next wall is perpendicular. The new wall normal is the lateral direction itself!
    FVector NewWallNormal = LateralDir; // The side wall faces toward us
    FVector TraceStart = PastEdge + (NewWallNormal * 80.f);
    FVector TraceEnd = PastEdge - (NewWallNormal * 80.f);

    FHitResult NextWallHit;
    bool bFoundNextWall = GetWorld()->LineTraceSingleByChannel(
        NextWallHit, TraceStart, TraceEnd, ECC_WorldStatic, Params
    );

    if (bShowDebugTraces)
    {
        DrawDebugLine(GetWorld(), TraceStart, TraceEnd, bFoundNextWall ? FColor::Green : FColor::Red, false, 2.f, 0, 3.f);
        DrawDebugSphere(GetWorld(), PastEdge, 15.f, 8, FColor::Yellow, false, 2.f);
    }

    if (!bFoundNextWall)
    {
        UE_LOG(LogHeroLedge, Log, TEXT("TryCornerWrap: No wall found around corner"));
        if (GEngine) GEngine->AddOnScreenDebugMessage(802, 2.f, FColor::Red, TEXT("[CORNER] No wall around corner!"));
        return false;
    }

    // ── Check ledge top on new wall ──
    FHitResult NewLedgeHit;
    FVector NewForward = -NextWallHit.ImpactNormal;
    if (!TraceWallTop(NextWallHit.ImpactPoint, NewForward, MaxGrabHeight, NewLedgeHit))
    {
        UE_LOG(LogHeroLedge, Log, TEXT("TryCornerWrap: No ledge top on new wall"));
        return false;
    }

    // ── Update hang state to new wall ──
    CurrentHangWallNormal = NextWallHit.ImpactNormal;
    CurrentHangLedgeTop = NewLedgeHit.ImpactPoint;

    UCapsuleComponent* Cap = Owner->FindComponentByClass<UCapsuleComponent>();
    float CapsuleHH = Cap ? Cap->GetScaledCapsuleHalfHeight() : 90.f;
    float ZOffset = -(CapsuleHH * 0.6f);

    FVector HangOffset = FVector(
        -CurrentHangWallNormal.X * 35.f,
        -CurrentHangWallNormal.Y * 35.f,
        ZOffset
    );
    CurrentHangTargetLocation = CurrentHangLedgeTop + HangOffset;
    CurrentHangTargetRotation = (-CurrentHangWallNormal).Rotation();
    CurrentHangTargetRotation.Pitch = 0.f;
    CurrentHangTargetRotation.Roll = 0.f;

    // ── Play Turn90 montage ──
    USkeletalMeshComponent* Mesh = Owner->FindComponentByClass<USkeletalMeshComponent>();
    UAnimInstance* AnimInst = Mesh ? Mesh->GetAnimInstance() : nullptr;

    UAnimMontage* TurnMontage = nullptr;
    if (bGoingRight)
        TurnMontage = TraversalDataAsset->TurnRight90_Hang_Montage
            ? TraversalDataAsset->TurnRight90_Hang_Montage
            : TraversalDataAsset->TurnRight90_Montage;
    else
        TurnMontage = TraversalDataAsset->TurnLeft90_Hang_Montage
            ? TraversalDataAsset->TurnLeft90_Hang_Montage
            : TraversalDataAsset->TurnLeft90_Montage;

    bIsPerformingAction = true;

    if (AnimInst && TurnMontage)
    {
        AnimInst->StopAllMontages(0.1f);
        float PlayResult = AnimInst->Montage_Play(TurnMontage, 1.0f);
        UE_LOG(LogHeroLedge, Log, TEXT("TryCornerWrap: Turn %s '%s' result=%.2f"),
            bGoingRight ? TEXT("RIGHT") : TEXT("LEFT"), *TurnMontage->GetName(), PlayResult);

        if (PlayResult > 0.f)
        {
            FOnMontageEnded EndDelegate;
            EndDelegate.BindUObject(this, &ULedgeDetectorComponent::OnActionMontageEnded);
            AnimInst->Montage_SetEndDelegate(EndDelegate, TurnMontage);
        }
    }

    // Snap to new corner position
    Owner->SetActorLocation(CurrentHangTargetLocation, false, nullptr, ETeleportType::TeleportPhysics);
    Owner->SetActorRotation(CurrentHangTargetRotation);

    if (GEngine) GEngine->AddOnScreenDebugMessage(802, 2.f, FColor::Magenta,
        FString::Printf(TEXT("[CORNER] Wrapped %s!"), bGoingRight ? TEXT("Right →") : TEXT("← Left")));

    return true;
}


// ═══════════════════════════════════════════════════════════════════
// ACTION MONTAGE END — return to hang loop after shimmy/corner
// ═══════════════════════════════════════════════════════════════════
void ULedgeDetectorComponent::OnActionMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    bIsPerformingAction = false;

    UE_LOG(LogHeroLedge, Log, TEXT("OnActionMontageEnded: '%s' bInterrupted=%d"),
        Montage ? *Montage->GetName() : TEXT("NULL"), bInterrupted);

    // ★ Naya Langar (New Anchor): Save current position as the new 'Safe Spot'
    AActor* Owner = GetOwner();
    if (Owner && !bInterrupted)
    {
        CurrentHangTargetLocation = Owner->GetActorLocation();
        CurrentHangTargetRotation = Owner->GetActorRotation();
    }

    if (!bIsHanging) return;

    // Return to hang loop
    USkeletalMeshComponent* Mesh = Owner ? Owner->FindComponentByClass<USkeletalMeshComponent>() : nullptr;
    UAnimInstance* AnimInst = Mesh ? Mesh->GetAnimInstance() : nullptr;

    UAnimMontage* LoopMontage = TraversalDataAsset ? TraversalDataAsset->LedgeHang_Free_Loop_Montage : nullptr;

    if (AnimInst && LoopMontage)
    {
        float LoopResult = AnimInst->Montage_Play(LoopMontage, 1.0f);
        if (LoopResult > 0.f)
        {
            FOnMontageEnded EndDelegate;
            EndDelegate.BindUObject(this, &ULedgeDetectorComponent::OnHangMontageEnded);
            AnimInst->Montage_SetEndDelegate(EndDelegate, LoopMontage);
        }
    }
}


// ═══════════════════════════════════════════════════════════════════
// EXISTING STUBS (CheckForCorner now has real implementation above via TryCornerWrap)
// ═══════════════════════════════════════════════════════════════════
bool ULedgeDetectorComponent::CheckForCorner(bool bCheckLeft, FVector& OutCornerLocation, FRotator& OutCornerRotation) { return false; }
bool ULedgeDetectorComponent::TraceBackClearance(const FVector& LedgeTopLocation, const FVector& Forward, float CapsuleHalfHeight, float Radius, FVector& OutBackFloor, FVector& OutBackLedge) const { return false; }
void ULedgeDetectorComponent::SetOverlayState(EHeroOverlayState NewState) {}

