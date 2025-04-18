// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HSBaseHiderCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Engine/EngineTypes.h"
#include "Net/UnrealNetwork.h"  // DOREPLIFETIME 매크로 정의
#include "Animation/AnimMontage.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/HSHiderAnimInstance.h"

AHSBaseHiderCharacter::AHSBaseHiderCharacter()
{
	// HiderCharacter = GameTraceChannel2
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);

    static ConstructorHelpers::FObjectFinder<UAnimMontage> MontObj(TEXT("/Game/Animation/Montage/AM_Seeker_Attack.AM_Seeker_Attack"));
    if (MontObj.Succeeded())
    {
        DeathMontage = MontObj.Object;
    }

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Game/Creative_Characters_FREE/Skeleton_Meshes/SK_Body_010.SK_Body_010"));
    if (CharacterMeshRef.Object)
    {
        GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
    }

    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Animation/ABP_HSHiderCharacter.ABP_HSHiderCharacter_C"));
    if (AnimInstanceClassRef.Class)
    {
        GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
    }
}

void AHSBaseHiderCharacter::MarkAsDead()
{
    if (bIsDead) return;
    bIsDead = true;

    if (HasAuthority())
    {
        HandleDeathVisuals(); // 서버도 직접 호출!
    }
}

void AHSBaseHiderCharacter::EnterDeathPose()
{
    // 1. 애니메이션 정지 (현재 프레임 유지)
    GetMesh()->bPauseAnims = true;
    GetMesh()->bNoSkeletonUpdate = true;

    // 4. 컨트롤러 분리 (AI/플레이어 입력 차단)
    DetachFromControllerPendingDestroy();

    // 5. 위치 복제 중단 (서버 위치 덜 갱신되게)
    SetReplicateMovement(false);

    // 6. 필요 시, 시체 타이머로 Destroy 예약
    // SetLifeSpan(5.0f);
}

void AHSBaseHiderCharacter::OnRep_bIsDead()
{
    if (bIsDead)
    {
        HandleDeathVisuals(); // 클라이언트는 Rep 통해 자동 호출
    }
}

void AHSBaseHiderCharacter::HandleDeathVisuals()
{
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetCharacterMovement()->DisableMovement();

    if (DeathMontage)
    {
        PlayAnimMontage(DeathMontage);
    }
}

void AHSBaseHiderCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AHSBaseHiderCharacter, bIsDead);
}

