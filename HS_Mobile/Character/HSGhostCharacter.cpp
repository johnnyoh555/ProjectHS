// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HSGhostCharacter.h"
#include "Components/CapsuleComponent.h"


void AHSGhostCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitCapsule();
	InitMesh();
}

void AHSGhostCharacter::InitCapsule()
{
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Capsule->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void AHSGhostCharacter::InitMesh()
{
	if (!IsLocallyControlled())
	{
		GetMesh()->SetVisibility(false, true); // 전체 하위까지 숨김
	}
}

bool AHSGhostCharacter::IsNetRelevantFor(const AActor* RealViewer, const AActor* Viewer, const FVector& SrcLocation) const
{
	// 나를 조종 중인 PlayerController에만 Relevant
	return (Viewer == GetOwner());
}
