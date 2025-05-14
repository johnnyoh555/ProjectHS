// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "HSMissionRow.generated.h"

USTRUCT(BlueprintType)
struct FHSMissionRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UHSMissionBase> MissionClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MissionId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText MissionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GoalCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AssociatedTriggerId;
	
};