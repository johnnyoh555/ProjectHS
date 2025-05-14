// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mission/HSMissionBase.h"
#include "HSMissionTest.generated.h"

/**
 * 
 */
UCLASS()
class HS_MOBILE_API UHSMissionTest : public UHSMissionBase
{
	GENERATED_BODY()
	
public:
	UHSMissionTest();
	virtual void StartMission(const FName& Id) override;
};
