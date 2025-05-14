// Fill out your copyright notice in the Description page of Project Settings.


#include "Mission/Missions/HSMissionTest.h"



UHSMissionTest::UHSMissionTest()
{
	MissionId = "test";
	MissionName = FText::FromString("Test Mission");
	GoalCount = 5;
	CurrentCount = 0;
	bIsCompleted = false;
}

void UHSMissionTest::StartMission(const FName& Id)
{
	UE_LOG(LogTemp, Log, TEXT("Mission %s started!"), *Id.ToString());
}
