// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/HSPlayerMissionManagerComponent.h"
#include "Interface/HSMissionTriggerInterface.h"
#include "Mission/HSMissionBase.h"
#include "Mission/HSMissionProgressTypes.h"
#include "Net/UnrealNetwork.h"

UHSPlayerMissionManagerComponent::UHSPlayerMissionManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UHSPlayerMissionManagerComponent::AddMission(UHSMissionBase* NewMission)
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;

	if (NewMission)
	{
		FMissionProgress NewProgress;
		NewProgress.MissionId = NewMission->MissionId;
		NewProgress.GoalCount = NewMission->GoalCount;
		NewProgress.CurrentCount = 0;

		MissionProgresses.Add(NewProgress);
		Missions.Add(NewMission->MissionId, NewMission);
		UE_LOG(LogTemp, Log, TEXT("AddMission!!! %s added!"), *NewMission->MissionId.ToString());
	}
}


void UHSPlayerMissionManagerComponent::HandleMissionInteract(const FName& MissionId)
{
	if (!GetOwner()->HasAuthority() || MissionId.IsNone()) return;

	FMissionProgress* Progress = FindMissionProgressById(MissionId);
	UHSMissionBase* MissionObject = FindMissionObject(MissionId);

	if (Progress && MissionObject)
	{
		if (!Progress->bCompleted)
		{
			MissionObject->StartMission(MissionId);
		}
	}
}

FMissionProgress* UHSPlayerMissionManagerComponent::FindMissionProgressById(const FName& MissionId)
{
	if (FMissionProgress* FoundProgress = MissionProgresses.FindByPredicate([&](const FMissionProgress& Progress) { return Progress.MissionId == MissionId; }))
	{
		return FoundProgress;
	}
	return nullptr;

}

UHSMissionBase* UHSPlayerMissionManagerComponent::FindMissionObject(const FName& MissionId) const
{
	if (const TObjectPtr<UHSMissionBase>* FoundMission = Missions.Find(MissionId))
	{
		return FoundMission->Get();
	}
	return nullptr;
}

bool UHSPlayerMissionManagerComponent::CanInteractMission(const FName& MissionId) const
{
	if (const bool* bCompleted = MissionProgressCompleted.Find(MissionId))
	{
		return *bCompleted ? false : true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MissionId %s not found in MissionProgressCompleted, %d"), *MissionId.ToString(), MissionProgressCompleted.Num());
	}
	return false;
}

void UHSPlayerMissionManagerComponent::OnRep_MissionProgresses()
{
	for (const FMissionProgress& Progress : MissionProgresses)
	{
		bool& bCompleted = MissionProgressCompleted.FindOrAdd(Progress.MissionId);
		bCompleted = Progress.bCompleted;
		UE_LOG(LogTemp, Log, TEXT("MissionId %s added to MissionProgressCompleted, %d"), *Progress.MissionId.ToString(), MissionProgressCompleted.Num());
	}
}

void UHSPlayerMissionManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHSPlayerMissionManagerComponent, MissionProgresses);
}