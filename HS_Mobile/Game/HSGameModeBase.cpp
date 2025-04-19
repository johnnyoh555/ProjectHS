// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HSGameModeBase.h"
#include "HSGameModeBase.h"
#include "Character/HSSeekerCharacter.h"
#include "Character/HSBaseHiderCharacter.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/GameStateBase.h"

AHSGameModeBase::AHSGameModeBase()
{
	// Pawn 지정
    static ConstructorHelpers::FClassFinder<APawn> HiderBPClass(TEXT("/Game/Character/BP_HSHiderCharacter.BP_HSHiderCharacter_C"));
    if (HiderBPClass.Class)
    {
        DefaultPawnClass = HiderBPClass.Class;
        HiderClass = HiderBPClass.Class; // 추가!
    }

    // 술래 캐릭터 블루프린트
    static ConstructorHelpers::FClassFinder<APawn> SeekerBPClass(TEXT("/Game/Character/BP_HSSeekerCharacter.BP_HSSeekerCharacter_C"));
    if (SeekerBPClass.Class)
    {
        SeekerClass = SeekerBPClass.Class; // 추가!
    }

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/HS_Mobile.HSPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}

void AHSGameModeBase::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    if (!NewPlayer || !HiderClass || !SeekerClass) return;

    APawn* NewPawn = nullptr;

    const int32 CurrentPlayerIndex = (GameState && GameState->PlayerArray.Num() > 0) ? GameState->PlayerArray.Num() - 1 : 0;

    if (CurrentPlayerIndex == 0)
    {
        // 첫 번째 플레이어는 술래
        NewPawn = GetWorld()->SpawnActor<AHSSeekerCharacter>(SeekerClass, SeekerSpawnTransform);
    }
    else
    {
        // 나머지는 도망자
        NewPawn = GetWorld()->SpawnActor<AHSBaseHiderCharacter>(HiderClass, HiderSpawnTransform);
    }

    if (NewPawn)
    {
        NewPlayer->Possess(NewPawn);
    }
}
