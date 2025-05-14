// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/HSBaseCharacter.h"
#include "HSGhostCharacter.generated.h"

/**
 * 유령 캐릭터 클래스: 사망 또는 탈출 후 관전용 상태로 전환될 때 사용
 * - Z축 이동 없음 (BaseCharacter 이동 로직 유지)
 * - 점프/상호작용 미사용
 * - 다른 클라이언트에게는 보이지 않음
 */
UCLASS()
class HS_MOBILE_API AHSGhostCharacter : public AHSBaseCharacter
{
	GENERATED_BODY()

public:
	AHSGhostCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual bool IsNetRelevantFor(const AActor* RealViewer, const AActor* Viewer, const FVector& SrcLocation) const override;

	virtual void Move(const FInputActionValue& Value) override;
	void MoveStop();
};
