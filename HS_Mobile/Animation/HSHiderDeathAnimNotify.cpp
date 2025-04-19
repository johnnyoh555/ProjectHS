// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/HSHiderDeathAnimNotify.h"
#include "HSHiderDeathAnimNotify.h"
#include "Character/HSBaseHiderCharacter.h"

void UHSHiderDeathAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (AActor* Owner = MeshComp->GetOwner())
    {
        if (AHSBaseHiderCharacter* Hider = Cast<AHSBaseHiderCharacter>(Owner))
        {
            Hider->EnterDeathPoseNotify();
        }
    }
}
