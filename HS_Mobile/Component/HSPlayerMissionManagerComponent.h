// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HSPlayerMissionManagerComponent.generated.h"

class AHSMissionTrigger;
class UHSMissionBase;
struct FMissionProgress;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HS_MOBILE_API UHSPlayerMissionManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHSPlayerMissionManagerComponent();

	void AddMission(UHSMissionBase* NewMission);

	// 미션과 관련된 서버 처리
	void HandleMissionInteract(const FName& MissionId);

	// PlayerState가 가지고 있는 MissionProgress 조회
	FMissionProgress* FindMissionProgressById(const FName& MissionId);

	// MissionBase 객체 조회
	UHSMissionBase* FindMissionObject(const FName& MissionId) const;

	bool CanInteractMission(const FName& MissionId) const;

private:
	// 미션 진행 상황을 복제하는 배열
	UPROPERTY(ReplicatedUsing = OnRep_MissionProgresses)
	TArray<FMissionProgress> MissionProgresses;

	// 서버 전용 미션 객체 배열
	UPROPERTY()
	TMap<FName, TObjectPtr<UHSMissionBase>> Missions;

	TMap<FName, bool> MissionProgressCompleted;

	UFUNCTION()
	void OnRep_MissionProgresses();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
