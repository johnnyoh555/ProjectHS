// Fill out your copyright notice in the Description page of Project Settings.


#include "Mission/HSMissionBase.h"
#include "Mission/HSMissionRow.h"


void UHSMissionBase::Initialize(const FHSMissionRow& Def)
{
	MissionId = Def.MissionId;
	MissionName = Def.MissionName;
	GoalCount = Def.GoalCount;
	CurrentCount = 0;
	bIsCompleted = false;
}

void UHSMissionBase::UpdateProgress(int32 Amount)
{
	if (bIsCompleted) return;

	CurrentCount += Amount;
	if (CurrentCount >= GoalCount)
	{
		CompleteMission();
	}
}

void UHSMissionBase::CompleteMission()
{
	bIsCompleted = true;
	// Notify completion (e.g., through an event or delegate)
	UE_LOG(LogTemp, Log, TEXT("Mission %s completed!"), *MissionId.ToString());
}
