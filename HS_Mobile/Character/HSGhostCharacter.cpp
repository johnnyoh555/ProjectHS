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
	bIsRunning = true; // 기본적으로 달리기 상태로 시작

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
}

void AHSGhostCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Beginplay에서 캐릭터 스탯 데이터 설정을 하기 때문에 다시 여기서 선언
	// 별도 Z 고정 로직 필요 없음
	GetCharacterMovement()->GravityScale = 0.f;
	GetCharacterMovement()->Velocity = FVector::ZeroVector;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;

	// 브레이킹 설정 강화
	GetCharacterMovement()->BrakingDecelerationFlying = 10000.0f; // 초당 감속률
	GetCharacterMovement()->BrakingFriction = 15.0f; // 감속이 있을때 마찰률
	GetCharacterMovement()->bUseSeparateBrakingFriction = true;
}

void AHSGhostCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// 점프 및 상호작용은 미사용
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHSGhostCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AHSGhostCharacter::Look);
		//EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AHSGhostCharacter::MoveStop);
	}
}

// Tick은 사용하지 않고, Move() 안에서 감속 처리를 유도
void AHSGhostCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	// 입력 벡터가 거의 0이면 Stop 처리
	if (MovementVector.IsNearlyZero())
	{
		// MoveStop(); // 감속
		GetCharacterMovement()->Velocity = FVector::ZeroVector; // ✅ 관성 제거
		return;
	}

	// 방향 계산 및 이동 입력
	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.X);
		AddMovementInput(RightDirection, MovementVector.Y);
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
