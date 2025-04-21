// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/HSGhostCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"

AHSGhostCharacter::AHSGhostCharacter()
{
	PrimaryActorTick.bCanEverTick = false; // Tick 제거

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Game/Creative_Characters_FREE/Skeleton_Meshes/SK_Body_010.SK_Body_010"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	// 캡슐 충돌: 지형만 밟고, 다른 캐릭터는 무시함
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block); // 지형은 Block
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore); // 다른 캐릭터 무시

	// 메시 충돌 제거
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// Z축 고정: 점프 불가능하고 Z 이동 없는 BaseCharacter 기반이므로 별도 처리 생략
}

void AHSGhostCharacter::BeginPlay()
{
	Super::BeginPlay();
	// 별도 Z 고정 로직 필요 없음
	GetCharacterMovement()->GravityScale = 0.f;
	GetCharacterMovement()->Velocity = FVector::ZeroVector;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void AHSGhostCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// 점프 및 상호작용은 미사용
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHSGhostCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AHSGhostCharacter::Look);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AHSGhostCharacter::MoveStop);
	}
}

void AHSGhostCharacter::MoveStop()
{
	GetCharacterMovement()->StopMovementImmediately();
	ConsumeMovementInputVector();
	UE_LOG(LogTemp, Warning, TEXT("MoveStop: Velocity = %s"), *GetCharacterMovement()->Velocity.ToString());
}

bool AHSGhostCharacter::IsNetRelevantFor(const AActor* RealViewer, const AActor* Viewer, const FVector& SrcLocation) const
{
	// 자기 자신에게만 보이도록
	return (RealViewer == this || Viewer == this);
}
