// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/HSBaseHiderCharacter.h"
#include "HSPlayerHiderCharacter.generated.h"

/**
 * 
 */
UCLASS()
class HS_MOBILE_API AHSPlayerHiderCharacter : public AHSBaseHiderCharacter
{
	GENERATED_BODY()
	
protected:
	virtual void EnterDeathNotify() override;
};
