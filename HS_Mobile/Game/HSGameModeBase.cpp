// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HSGameModeBase.h"
#include "HSGameModeBase.h"

AHSGameModeBase::AHSGameModeBase()
{
	// Pawn 지정
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Game/Character/BP_HSHiderCharacter.BP_HSHiderCharacter_C"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/HS_Mobile.HSPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}
