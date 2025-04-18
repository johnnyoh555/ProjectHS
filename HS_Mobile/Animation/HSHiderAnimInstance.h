// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/HSAnimInstance.h"
#include "HSHiderAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class HS_MOBILE_API UHSHiderAnimInstance : public UHSAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	bool bIsDead;

};
