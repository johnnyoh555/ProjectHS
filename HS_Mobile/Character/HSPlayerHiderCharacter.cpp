// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/HSPlayerHiderCharacter.h"
#include "Character/HSGhostCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Component/HSMissionInteractComponent.h"

AHSPlayerHiderCharacter::AHSPlayerHiderCharacter()
{
	MissionInteractComponent = CreateDefaultSubobject<UHSMissionInteractComponent>(TEXT("MissionInteractComponent"));
}

void AHSPlayerHiderCharacter::StartInteract()
{
	if (MissionInteractComponent)
	{
		MissionInteractComponent->RequestInteract();
		UE_LOG(LogTemp, Warning, TEXT("HiderMissionInteract!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MissionInteractComponent is not initialized!"));
	}
}

void AHSPlayerHiderCharacter::EnterDeathNotify()
{
	Super::EnterDeathNotify();

	if (HasAuthority())
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			FTransform SpawnTransform = GetActorTransform();
			SpawnTransform.AddToTranslation(FVector(0.f, 0.f, 100.f));
			AHSGhostCharacter* Ghost = GetWorld()->SpawnActor<AHSGhostCharacter>(AHSGhostCharacter::StaticClass(), SpawnTransform, SpawnParams);

			if (Ghost)
			{
				PC->Possess(Ghost);
				UE_LOG(LogTemp, Warning, TEXT("SetAsDead: Possess Ghost Character"));
			}
		}
	}
}