// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "HSAIHiderController.generated.h"

/**
 * 
 */
UCLASS()
class HS_MOBILE_API AHSAIHiderController : public AAIController
{
	GENERATED_BODY()
	
public:
	AHSAIHiderController();

protected:
	virtual void OnPossess(APawn* InPawn) override;

	FVector GetRandomPointInNavMesh();
	void MoveToRandomPoint();
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	FTimerHandle MovementTimerHandle;

	float MinRadius = 300.f; // 최소 반경
	float MaxRadius = 1500.f; // 최대 반경
};
