// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/HSSeekerAttackComponent.h"
#include "Components/BoxComponent.h"
#include "Character/HSBaseHiderCharacter.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"  // DOREPLIFETIME 매크로 정의

UHSSeekerAttackComponent::UHSSeekerAttackComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);

    // 박스 컴포넌트 생성
    AttackBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackBox"));
    AttackBox->SetBoxExtent(FVector(60.f, 70.f, 80.f)); // 원하는 크기로 조절
    AttackBox->SetCollisionProfileName(TEXT("AttackTrigger"));
    AttackBox->SetGenerateOverlapEvents(true);
    AttackBox->SetHiddenInGame(false); // 개발 중엔 영역이 보이게

    static ConstructorHelpers::FObjectFinder<UAnimMontage> MontObj(TEXT("/Game/Animation/Montage/AM_Seeker_Attack.AM_Seeker_Attack"));
    if (MontObj.Succeeded())
    {
        AttackMontage = MontObj.Object;
    }
}

void UHSSeekerAttackComponent::OnRegister()
{
    Super::OnRegister();

    if (AActor* Owner = GetOwner())
    {
        if (USceneComponent* Root = Owner->GetRootComponent())
        {
            AttackBox->AttachToComponent(Root, FAttachmentTransformRules::SnapToTargetIncludingScale);
            AttackBox->SetRelativeLocation(FVector(50.f, 0.f, 0.f)); // 캐릭터 앞쪽
        }
    }
}

void UHSSeekerAttackComponent::BeginPlay()
{
    Super::BeginPlay();

    AttackBox->OnComponentBeginOverlap.AddDynamic(this, &UHSSeekerAttackComponent::OnAttackBoxBeginOverlap);
    AttackBox->OnComponentEndOverlap.AddDynamic(this, &UHSSeekerAttackComponent::OnAttackBoxEndOverlap);
}

void UHSSeekerAttackComponent::OnAttackBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (AHSBaseHiderCharacter* Hider = Cast<AHSBaseHiderCharacter>(OtherActor))
    {
        DetectedTargets.Add(Hider);
    }

    if (DetectedTargets.Num() > 0)
    {
        AttackBox->SetHiddenInGame(true);
    }
}

void UHSSeekerAttackComponent::OnAttackBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (AHSBaseHiderCharacter* Hider = Cast<AHSBaseHiderCharacter>(OtherActor))
    {
        DetectedTargets.Remove(Hider);
    }

	if (DetectedTargets.Num() == 0)
    {
        AttackBox->SetHiddenInGame(false); 
    }
}

// 공격 요청 (클라이언트 -> 서버)
void UHSSeekerAttackComponent::RequestAttack()
{
    if (GetOwner()->HasAuthority())
    {
        PerformAttack();
    }
    else
    {
        ServerPerformAttack();
    }
}

// 서버 RPC 함수
void UHSSeekerAttackComponent::ServerPerformAttack_Implementation()
{
    PerformAttack();
}

void UHSSeekerAttackComponent::MulticastPlayAttackMontage_Implementation()
{
    if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
    {
        UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
        if (AnimInstance && AttackMontage)
        {
            AnimInstance->Montage_Play(AttackMontage);
            UE_LOG(LogTemp, Warning, TEXT("🔥 Multicast로 몽타주 재생!"));
        }
    }
}

// 실제 공격 수행
void UHSSeekerAttackComponent::PerformAttack()
{
    MulticastPlayAttackMontage();
}

void UHSSeekerAttackComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // 복제할 프로퍼티 등록
    DOREPLIFETIME(UHSSeekerAttackComponent, AttackMontage);
    // 필요하면 다른 Replicated 프로퍼티도 등록
}