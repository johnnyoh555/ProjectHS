// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/HSSeekerAttackHitAnimNotify.h"
#include "Component/HSSeekerAttackComponent.h"
#include "GameFramework/Actor.h"

void UHSSeekerAttackHitAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (AActor* Owner = MeshComp->GetOwner())
    {
        if (UHSSeekerAttackComponent* AttackComp = Owner->FindComponentByClass<UHSSeekerAttackComponent>())
        {
            AttackComp->HandleAttackHitNotify(); // 이 함수는 직접 만들어줘야 함
        }
    }
}
