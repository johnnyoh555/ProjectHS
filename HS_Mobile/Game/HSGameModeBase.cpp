// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HSGameModeBase.h"
#include "HSGameModeBase.h"
#include "Character/HSSeekerCharacter.h"
#include "Character/HSBaseHiderCharacter.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/GameStateBase.h"
#include "Controller/HSPlayerController.h"
#include "PlayerState/HSPlayerState.h"

AHSGameModeBase::AHSGameModeBase()
{
	// Pawn 지정
    static ConstructorHelpers::FClassFinder<APawn> HiderBPClass(TEXT("/Game/Character/BP_HSHiderCharacter.BP_HSHiderCharacter_C"));
    if (HiderBPClass.Class)
    {
        HiderClass = HiderBPClass.Class; // 추가!
    }

    // 술래 캐릭터 블루프린트
    static ConstructorHelpers::FClassFinder<APawn> SeekerBPClass(TEXT("/Game/Character/BP_HSSeekerCharacter.BP_HSSeekerCharacter_C"));
    if (SeekerBPClass.Class)
    {
        DefaultPawnClass = SeekerBPClass.Class;
        SeekerClass = SeekerBPClass.Class; // 추가!
    }

    PlayerControllerClass = AHSPlayerController::StaticClass();
    PlayerStateClass = AHSPlayerState::StaticClass();
}

void AHSGameModeBase::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    if (HiderClass != nullptr)
    {
        DefaultPawnClass = HiderClass;
    }
}
