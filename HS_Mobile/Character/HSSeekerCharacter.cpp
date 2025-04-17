// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HSSeekerCharacter.h"
#include "Component/HSSeekerAttackComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AHSSeekerCharacter::AHSSeekerCharacter()
{
	SeekerAttackComponent = CreateDefaultSubobject<UHSSeekerAttackComponent>(TEXT("SeekerAttackComponent"));
}

void AHSSeekerCharacter::StartInteract()
{
	if (SeekerAttackComponent)
	{
		SeekerAttackComponent->RequestAttack();
		UE_LOG(LogTemp, Warning, TEXT("SeekerAttack!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SeekerAttackComponent is not initialized!"));
	}
}

