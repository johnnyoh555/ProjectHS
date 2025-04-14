// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/HSAnimInstance.h"
#include "HSAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UHSAnimInstance::UHSAnimInstance()
{
	MovingThreshould = 5.0f;
	JumpingThreshould = 10.0f;
}

void UHSAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UHSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{	
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshould;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);
	}
}
