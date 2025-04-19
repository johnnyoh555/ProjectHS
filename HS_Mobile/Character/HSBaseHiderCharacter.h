// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/HSBaseCharacter.h"
#include "HSBaseHiderCharacter.generated.h"

class UAnimMontage;

/**
 * 도망자 캐릭터 - 사망 상태 및 연출 관리
 */
UCLASS()
class HS_MOBILE_API AHSBaseHiderCharacter : public AHSBaseCharacter
{
	GENERATED_BODY()

	public:
	AHSBaseHiderCharacter();

	// Actor 시작 시 처리
	virtual void BeginPlay() override;

	// 사망 상태 설정
	void SetAsDead();

	// 사망 여부 조회
	FORCEINLINE bool IsDead() const { return bIsDead; }

	// AnimNotify로부터 호출: 애니메이션 정지 및 스켈레톤 갱신 정지
	void EnterDeathPoseNotify();

protected:
	// 사망 상태 시 동작 (충돌 제거, 이동 불가 등)
	void ApplyDeathState();

	// 사망 몽타주 실행 (Multicast)
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayDeathMontage();

	// 사망 연출용 애니메이션 몽타주
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> DeathMontage;

	// 사망 여부 복제 변수
	UPROPERTY(Replicated)
	bool bIsDead = false;

	// 복제 설정
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
