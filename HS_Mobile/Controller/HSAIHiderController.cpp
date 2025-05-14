// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/HSAIHiderController.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"

AHSAIHiderController::AHSAIHiderController()
{
	bAttachToPawn = true;
}

void AHSAIHiderController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	MoveToLocation(GetPawn()->GetActorLocation());
}

FVector AHSAIHiderController::GetRandomPointInNavMesh()
{
	FVector AIAgentLocation = GetPawn()->GetActorLocation(); // GetActorLocation() → Pawn 기준으로
	FNavLocation RandomLocation;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSystem)
	{
		float RandomRadius = FMath::FRandRange(MinRadius, MaxRadius); // 거리 랜덤
		if (NavSystem->GetRandomReachablePointInRadius(AIAgentLocation, RandomRadius, RandomLocation))
		{
			return RandomLocation.Location;
		}
	}

	// 실패 시 현재 위치 반환 (안정성)
	return AIAgentLocation;
}

void AHSAIHiderController::MoveToRandomPoint()
{
	MoveToLocation(GetRandomPointInNavMesh());
}

void AHSAIHiderController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (Result.IsSuccess())
	{
		// 일정 확률로 멈추거나 다시 이동
		if (FMath::FRand() < 0.7f) // 70% 확률로 다시 이동
		{
			MoveToRandomPoint();
		}
		else
		{
			StopMovement();

			// 2~4초 후 다시 판단
			GetWorld()->GetTimerManager().SetTimer(MovementTimerHandle, this, &AHSAIHiderController::MoveToRandomPoint, FMath::FRandRange(2.f, 4.f), false);
		}
	}
}