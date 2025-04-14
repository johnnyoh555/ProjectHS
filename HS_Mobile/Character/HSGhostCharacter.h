// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/HSBaseCharacter.h"
#include "HSGhostCharacter.generated.h"

/**
 * 
 */
UCLASS()
class HS_MOBILE_API AHSGhostCharacter : public AHSBaseCharacter
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void InitCapsule() override;
	virtual void InitMesh() override;

	virtual bool IsNetRelevantFor(const AActor* RealViewer, const AActor* Viewer, const FVector& SrcLocation) const override;
};
