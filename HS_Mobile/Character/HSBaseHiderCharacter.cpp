// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/HSBaseHiderCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Engine/EngineTypes.h"
#include "Net/UnrealNetwork.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/HSHiderAnimInstance.h"

// 생성자: 콜리전 설정, 메쉬 및 애니메이션 초기화
AHSBaseHiderCharacter::AHSBaseHiderCharacter()
{
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

// BeginPlay에서 사망 상태 복제된 경우 즉시 시체 상태 반영
void AHSBaseHiderCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (!HasAuthority() && bIsDead)
    {
        ApplyDeathState();
	    UE_LOG(LogTemp, Warning, TEXT("BeginPlay: bIsDead = %s"), bIsDead ? TEXT("true") : TEXT("false"));
    }
}

// 사망 상태로 전환 (서버에서 Multicast 호출)
void AHSBaseHiderCharacter::SetAsDead()
{
    if (bIsDead) return;
    bIsDead = true;

    if (HasAuthority())
    {
        Multicast_PlayDeathMontage(); // 서버에서 애니메이션 연출 및 상태 처리 전파
    }
}

// 충돌 제거 및 이동 정지 처리
void AHSBaseHiderCharacter::ApplyDeathState()
{
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

    GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);

    GetCharacterMovement()->DisableMovement();

    //DetachFromControllerPendingDestroy(); // 컨트롤러와 분리하여 AI나 입력 제어 제거
    //SetReplicateMovement(false); // 위치 복제를 중단하여 죽은 후 서버 이동 동기화 방지
}

// 몽타주 재생을 모든 클라이언트에 전파 (서버 포함) + 상태 처리
void AHSBaseHiderCharacter::Multicast_PlayDeathMontage_Implementation()
{
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (DeathMontage && AnimInstance && !AnimInstance->Montage_IsPlaying(DeathMontage))
    {
        UE_LOG(LogTemp, Warning, TEXT("[Multicast] Play Death Montage!"));
        PlayAnimMontage(DeathMontage);
    }

    ApplyDeathState();
}

// AnimNotify를 통해 호출됨: 애니메이션 정지 및 스켈레톤 업데이트 중지
void AHSBaseHiderCharacter::EnterDeathNotify()
{
    SetLifeSpan(5.0f); // 5초 후 삭제
}

// bIsDead 변수 복제 설정
void AHSBaseHiderCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AHSBaseHiderCharacter, bIsDead);
}
