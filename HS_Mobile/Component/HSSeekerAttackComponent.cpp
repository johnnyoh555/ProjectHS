// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/HSSeekerAttackComponent.h"
#include "Components/BoxComponent.h"
#include "Character/HSBaseHiderCharacter.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h" // DOREPLIFETIME 매크로 정의

// === Constructor & Lifecycle ===

UHSSeekerAttackComponent::UHSSeekerAttackComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);

    AttackBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackBox"));
    AttackBox->SetBoxExtent(FVector(60.f, 70.f, 80.f));
    AttackBox->SetCollisionProfileName(TEXT("AttackTrigger"));
    AttackBox->SetGenerateOverlapEvents(true);
    AttackBox->SetHiddenInGame(false);

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
            AttackBox->SetRelativeLocation(FVector(50.f, 0.f, 0.f));
        }
    }
}

void UHSSeekerAttackComponent::BeginPlay()
{
    Super::BeginPlay();

    AttackBox->OnComponentBeginOverlap.AddDynamic(this, &UHSSeekerAttackComponent::OnAttackBoxBeginOverlap);
    AttackBox->OnComponentEndOverlap.AddDynamic(this, &UHSSeekerAttackComponent::OnAttackBoxEndOverlap);
}

// === 공격 감지 ===

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

AHSBaseHiderCharacter* UHSSeekerAttackComponent::FindNearestTarget()
{
    AHSBaseHiderCharacter* ClosestTarget = nullptr;
    float MinDistSqr = MAX_FLT;
    const FVector MyLocation = GetOwner()->GetActorLocation();

    for (AHSBaseHiderCharacter* Target : DetectedTargets)
    {
        if (!IsValid(Target) || Target->IsDead()) continue;

        const float DistSqr = FVector::DistSquared(Target->GetActorLocation(), MyLocation);
        if (DistSqr < MinDistSqr)
        {
            MinDistSqr = DistSqr;
            ClosestTarget = Target;
        }
    }
    return ClosestTarget;
}

// === 공격 요청 및 처리 ===

void UHSSeekerAttackComponent::RequestAttack()
{
    if (DetectedTargets.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("공격할 타겟이 없습니다."));
        return;
    }

    if (GetOwner()->HasAuthority())
    {
        PerformAttack();
    }
    else
    {
        ServerPerformAttack();
    }
}

void UHSSeekerAttackComponent::PerformAttack()
{
    MulticastPlayAttackMontage();
}

void UHSSeekerAttackComponent::HandleAttackHitNotify()
{
    if (!GetOwner()->HasAuthority()) return;

    AHSBaseHiderCharacter* Target = FindNearestTarget();
    if (Target && !Target->IsDead())
    {
        Target->SetAsDead();
    }
}

// === 쿨타임 처리 ===

void UHSSeekerAttackComponent::StartAttackCooldown()
{
    bCanAttack = false;

    GetWorld()->GetTimerManager().SetTimer(
        AttackCooldownHandle,
        this,
        &UHSSeekerAttackComponent::ResetAttackCooldown,
        CooldownTime, false
    );
}

void UHSSeekerAttackComponent::ResetAttackCooldown()
{
    bCanAttack = true;
}

void UHSSeekerAttackComponent::ClientStartLocalCooldown_Implementation()
{
    bLocalCanAttack = false;

    GetWorld()->GetTimerManager().SetTimer(
        LocalAttackCooldownHandle,
        [this]()
    {
        bLocalCanAttack = true;
    },
        CooldownTime, false
    );
}

// === 애니메이션 ===

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

// === 서버 RPC ===

void UHSSeekerAttackComponent::ServerPerformAttack_Implementation()
{
    if (!bCanAttack)
    {
        UE_LOG(LogTemp, Warning, TEXT("⛔ 쿨타임 중입니다."));
        return;
    }

    StartAttackCooldown();
    ClientStartLocalCooldown();
    PerformAttack();
}

// === 복제 설정 ===

void UHSSeekerAttackComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UHSSeekerAttackComponent, AttackMontage);
}

