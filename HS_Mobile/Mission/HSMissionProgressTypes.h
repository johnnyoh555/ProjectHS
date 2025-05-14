// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HSMissionProgressTypes.generated.h"

USTRUCT(BlueprintType)
struct FMissionProgress
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FName MissionId;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GoalCount = 1;

	UPROPERTY(BlueprintReadOnly)
	bool bCompleted = false;
};