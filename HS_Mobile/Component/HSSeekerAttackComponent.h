// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HSSeekerAttackComponent.generated.h"

// 전방 선언
class UBoxComponent;
class AHSBaseHiderCharacter;
class UAnimMontage;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HS_MOBILE_API UHSSeekerAttackComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHSSeekerAttackComponent();

protected:
    // lifecycle
    virtual void OnRegister() override;
    virtual void BeginPlay() override;

    // 공격 영역
    UPROPERTY(VisibleAnywhere, Category = "Attack")
    TObjectPtr<UBoxComponent> AttackBox;

    // 감지된 캐릭터 저장
    UPROPERTY()
    TSet<TObjectPtr<AHSBaseHiderCharacter>> DetectedTargets;

    // 오버랩 이벤트
    UFUNCTION()
    void OnAttackBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnAttackBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// 공격 요청
public:
    void RequestAttack();

protected:
    // 복제 속성 구현을 위해 오버라이드 선언…
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(Replicated, EditAnywhere, Category = "Attack")
    UAnimMontage* AttackMontage;

    UFUNCTION(Server, Reliable)
    void ServerPerformAttack();

    UFUNCTION(NetMulticast, Unreliable)
    void MulticastPlayAttackMontage();

    void PerformAttack();
};