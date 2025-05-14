#include "Character/HSAIHiderCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Controller/HSAIHiderController.h"
#include "TimerManager.h"
#include "AI/Navigation/NavigationAvoidanceTypes.h"

AHSAIHiderCharacter::AHSAIHiderCharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AHSAIHiderController::StaticClass();

	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceConsiderationRadius = 300.f;
}

void AHSAIHiderCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 달리기 상태를 일정 시간마다 전환
	GetWorld()->GetTimerManager().SetTimer(
		RunToggleTimerHandle,
		this,
		&AHSAIHiderCharacter::ToggleRunState,
		4.0f, true
	);
}

void AHSAIHiderCharacter::ToggleRunState()
{
	//if (!bIsMoving) return; // 멈춰있을 땐 달리기/걷기 전환하지 않음

	if (FMath::FRand() <= 0.3f)
	{
		ToggleRunning();
	}
}
