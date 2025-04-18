// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/HSHiderAnimInstance.h"
#include "Character/HSBaseHiderCharacter.h"


void UHSHiderAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

	AHSBaseHiderCharacter* Hider = Cast<AHSBaseHiderCharacter>(Owner);
    if (Hider)
    {
        bIsDead = Hider->IsDead(); // 캐릭터에서 직접 상태 가져오기
    }
}
