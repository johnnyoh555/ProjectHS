// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/HSBaseCharacter.h"
#include "HSSeekerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class HS_MOBILE_API AHSSeekerCharacter : public AHSBaseCharacter
{
	GENERATED_BODY()
	
public:
	AHSSeekerCharacter();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UHSSeekerAttackComponent> SeekerAttackComponent;

	virtual void StartInteract() override;
};
