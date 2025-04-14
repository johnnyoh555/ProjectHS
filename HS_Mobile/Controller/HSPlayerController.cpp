// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/HSPlayerController.h"
#include "HSPlayerController.h"

AHSPlayerController::AHSPlayerController()
{
}

void AHSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 게임이 시작하자 마자 뷰포트로 포인터가 이동
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
