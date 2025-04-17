// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HSGameModeBase.h"
#include "HSGameModeBase.h"
#include "Character/HSSeekerCharacter.h"
#include "Character/HSBaseHiderCharacter.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"

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

    if (!NewPlayer) return;

    APawn* NewPawn = nullptr;

    // 첫 번째 플레이어는 술래로 생성
    if (PlayerCount == 0)
    {
        // ASeekerCharacter는 술래 캐릭터
        NewPawn = GetWorld()->SpawnActor<AHSSeekerCharacter>(SeekerClass, SeekerSpawnTransform);
    }
    else
    {
        // AHiderCharacter는 도망자 캐릭터
        NewPawn = GetWorld()->SpawnActor<AHSBaseHiderCharacter>(HiderClass, HiderSpawnTransform);
    }

    if (NewPawn)
    {
        NewPlayer->Possess(NewPawn);
    }

    PlayerCount++;
}
