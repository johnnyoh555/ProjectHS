// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/HSBaseCharacter.h"
#include "HSBaseHiderCharacter.generated.h"

/**
 * 
 */
UCLASS()
class HS_MOBILE_API AHSBaseHiderCharacter : public AHSBaseCharacter
{
	GENERATED_BODY()

public:
	AHSBaseHiderCharacter();

	FORCEINLINE bool IsDead() const { return bIsDead; }
	void MarkAsDead();
	void EnterDeathPose();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(ReplicatedUsing = OnRep_bIsDead)
	bool bIsDead = false;

	UFUNCTION()
	void OnRep_bIsDead();

	void HandleDeathVisuals();
};
