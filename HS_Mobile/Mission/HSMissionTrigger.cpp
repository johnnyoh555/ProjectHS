// Fill out your copyright notice in the Description page of Project Settings.


#include "Mission/HSMissionTrigger.h"
#include "Components/BoxComponent.h"

// Sets default values
AHSMissionTrigger::AHSMissionTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true; // 나중에 네트워크 동기화 고려
	
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;

	TriggerBox->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3); // 예: MissionTrigger 채널

	// 초기 사이즈만 설정. 블루프린트에서 변경 가능
	TriggerBox->InitBoxExtent(FVector(100.f, 100.f, 100.f));
}



