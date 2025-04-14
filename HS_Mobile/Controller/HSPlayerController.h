// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class HS_MOBILE_API AHSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AHSPlayerController();

protected:
	virtual void BeginPlay() override;
	
};
