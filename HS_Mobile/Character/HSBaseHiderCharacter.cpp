// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HSBaseHiderCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Engine/EngineTypes.h"

AHSBaseHiderCharacter::AHSBaseHiderCharacter()
{
	// HiderCharacter = GameTraceChannel2
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
}
