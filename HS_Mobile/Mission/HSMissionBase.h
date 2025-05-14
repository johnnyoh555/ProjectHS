// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HSMissionBase.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class HS_MOBILE_API UHSMissionBase : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	FName MissionId;

	UPROPERTY(BlueprintReadOnly)
	FText MissionName;

	UPROPERTY(BlueprintReadOnly)
	int32 GoalCount = 1;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentCount = 0;

	UPROPERTY(BlueprintReadOnly)
	bool bIsCompleted = false;

	virtual void StartMission(const FName& Id) PURE_VIRTUAL(UHSMissionBase::StartMission, );

protected:
	virtual void Initialize(const struct FHSMissionRow& Def);

	// 반드시 자식에서 구현해야 하는 함수 (Pure Virtual)

	virtual void UpdateProgress(int32 Amount = 1);

	virtual void CompleteMission();
};
