// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HSMissionOwnerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHSMissionOwnerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HS_MOBILE_API IHSMissionOwnerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// 미션 진행 상태 조회
	virtual struct FMissionProgress* FindMissionProgressById(const FName& MissionId) = 0;

	// 상호작용 가능한지 검사
	virtual bool CanInteractMission(const FName& MissionId) const = 0;

	virtual void HandleMissionInteract(const FName& MissionId) = 0;
};
