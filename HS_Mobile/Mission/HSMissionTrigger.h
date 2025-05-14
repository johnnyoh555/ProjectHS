// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/HSMissionTriggerInterface.h"
#include "HSMissionTrigger.generated.h"

UCLASS()
class HS_MOBILE_API AHSMissionTrigger : public AActor, public IHSMissionTriggerInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHSMissionTrigger();

	// IHSMissionTriggerInterface 구현
	FORCEINLINE virtual FName GetTriggerMissionId() const override { UE_LOG(LogTemp, Log, TEXT("GetTriggerMissionId called!")); return MissionId; }

private:
	// 트리거가 담당하는 미션 ID
	UPROPERTY(EditAnywhere, Category = "Mission")
	FName MissionId;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger")
	TObjectPtr<class UBoxComponent> TriggerBox;

};
