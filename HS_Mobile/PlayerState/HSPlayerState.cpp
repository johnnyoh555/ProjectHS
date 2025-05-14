// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/HSPlayerState.h"
#include "Mission/HSMissionBase.h"
#include "Mission/HSMissionProgressTypes.h"
#include "Net/UnrealNetwork.h"
#include "Mission/Missions/HSMissionTest.h"
#include "Component/HSPlayerMissionManagerComponent.h"

AHSPlayerState::AHSPlayerState()
{
	bReplicates = true; // ✅ 이렇게 설정
	MissionManager = CreateDefaultSubobject<UHSPlayerMissionManagerComponent>(TEXT("MissionManager"));
}

void AHSPlayerState::BeginPlay()
{
	Super::BeginPlay();

	UHSMissionTest* DummyMission = NewObject<UHSMissionTest>(this, UHSMissionTest::StaticClass());
	if (MissionManager && DummyMission)
	{
		MissionManager->AddMission(DummyMission);
		UE_LOG(LogTemp, Log, TEXT("Mission %s added!"), *DummyMission->MissionId.ToString());
	}
}

void AHSPlayerState::AddMission(UHSMissionBase* NewMission)
{
	if (MissionManager && NewMission)
	{
		MissionManager->AddMission(NewMission);
	}
}

FMissionProgress* AHSPlayerState::FindMissionProgressById(const FName& MissionId)
{
	if (MissionManager)
	{
		return MissionManager->FindMissionProgressById(MissionId);
	}
	return nullptr;
}

bool AHSPlayerState::CanInteractMission(const FName& MissionId) const
{
	if (MissionManager)
	{
		return MissionManager->CanInteractMission(MissionId);
	}
	return false;
}

void AHSPlayerState::HandleMissionInteract(const FName& MissionId)
{
	if (MissionManager)
	{
		MissionManager->HandleMissionInteract(MissionId);
	}
}

void AHSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 여기다가 복제할 변수를 등록
	DOREPLIFETIME(AHSPlayerState, PlayerRole);
}


