// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/HSBaseCharacter.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/HSCharacterStatDataAsset.h"
#include "Component/HSNetRelevancySightComponent.h" // 직접 생성할 .h
#include "Components/SphereComponent.h"

// Sets default values
AHSBaseCharacter::AHSBaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// 모바일 환경고려 틱 최소화
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicateMovement(true);
	NetCullDistanceSquared = FMath::Square(3000.f); // 기본 거리 컷 (필요시 오버라이드)

	InitCapsule();
	InitMesh();
	InitCamera();
	InitInputMapping();
	InitComponent();
}

// Called when the game starts or when spawned
void AHSBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetCharacterStatData();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		// 로컬 플레이어의 입력 매핑을 관리하는 서브시스템
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}
}

void AHSBaseCharacter::InitCapsule()
{
	GetCapsuleComponent()->InitCapsuleSize(40.f, 80.0f);
	GetCapsuleComponent()->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	/*GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));*/

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCameraBlock"));
	GetMesh()->SetCollisionProfileName(TEXT("NoCameraBlock"));
}

void AHSBaseCharacter::InitMesh()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT(""));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT(""));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -78.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
}

void AHSBaseCharacter::InitCamera()
{
	// 카메라 붐을 생성하고 루트 컴포넌트에 부착합니다.
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void AHSBaseCharacter::InitInputMapping()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_HSTPCharacter.IMC_HSTPCharacter'"));
	if (nullptr != InputMappingContextRef.Object)
	{
		InputMapping = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_TPMove.IA_TPMove'"));
	if (nullptr != InputActionMoveRef.Object)
	{
		MoveAction = InputActionMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_TPLook.IA_TPLook'"));
	if (nullptr != InputActionLookRef.Object)
	{
		LookAction = InputActionLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionRunRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_Run.IA_Run'"));
	if (nullptr != InputActionRunRef.Object)
	{
		RunAction = InputActionRunRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionInteractRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_Interact.IA_Interact'"));
	if (nullptr != InputActionInteractRef.Object)
	{
		InteractAction = InputActionInteractRef.Object;
	}

}

void AHSBaseCharacter::InitComponent()
{
	// 캐릭터 스탯 데이터 에셋을 로드합니다.
	static ConstructorHelpers::FObjectFinder<UHSCharacterStatDataAsset> StatDataAssetRef(TEXT("/Script/HS_Mobile.HSCharacterStatDataAsset'/Game/Character/HSBaseCharacterStat.HSBaseCharacterStat'"));
	if (StatDataAssetRef.Object)
	{
		CharacterStatDataAsset = StatDataAssetRef.Object;
	}

	NetRelevancySightComponent = CreateDefaultSubobject<UHSNetRelevancySightComponent>(TEXT("NetRelevancySightComponent"));
}

void AHSBaseCharacter::SetCharacterStatData()
{
	if (!CharacterStatDataAsset)
	{
		// CharacterStatDataAsset이 nullptr인 경우 처리
		// 예를 들어, 로그를 출력하거나 기본값을 설정하는 등의 작업을 수행할 수 있습니다.
		UE_LOG(LogTemp, Warning, TEXT("CharacterStatDataAsset not found!"));
		return;
	}

	// Pawn
	bUseControllerRotationYaw = CharacterStatDataAsset->bUseControllerRotationYaw;

	//// Character
	GetCharacterMovement()->bOrientRotationToMovement = CharacterStatDataAsset->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterStatDataAsset->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterStatDataAsset->RotationRate;
	GetCharacterMovement()->JumpZVelocity = CharacterStatDataAsset->JumpZVelocity;
	GetCharacterMovement()->AirControl = CharacterStatDataAsset->AirControl;
	GetCharacterMovement()->MaxWalkSpeed = CharacterStatDataAsset->MaxWalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = CharacterStatDataAsset->MinAnalogWalkSpeed;
	GetCharacterMovement()->BrakingDecelerationWalking = CharacterStatDataAsset->BrakingDecelerationWalking;
	GetCharacterMovement()->GravityScale = CharacterStatDataAsset->GravityScale;

	CameraBoom->TargetArmLength = CharacterStatDataAsset->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterStatDataAsset->RelativeRotation);
	CameraBoom->SetRelativeLocation(CharacterStatDataAsset->RelativeLocationRate);
	CameraBoom->bUsePawnControlRotation = CharacterStatDataAsset->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterStatDataAsset->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterStatDataAsset->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterStatDataAsset->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterStatDataAsset->bDoCollisionTest;
	CameraBoom->ProbeChannel = ECollisionChannel::ECC_GameTraceChannel1; // CameraCollision 채널 번호 확인

	WalkSpeed = CharacterStatDataAsset->WalkSpeed;
	RunSpeed = CharacterStatDataAsset->RunSpeed;
}

// Called to bind functionality to input
void AHSBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHSBaseCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AHSBaseCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AHSBaseCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AHSBaseCharacter::StopJumping);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AHSBaseCharacter::StartInteract);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &AHSBaseCharacter::ToggleRunning);
	}
}

void AHSBaseCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

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

void AHSBaseCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AHSBaseCharacter::ToggleRunning()
{
	bIsRunning = !bIsRunning; // 상태 뒤집기
	UpdateMovementSpeed();
	ServerSetRunning(bIsRunning);
}

void AHSBaseCharacter::Jump()
{
	Super::Jump();
	bIsInAir = true; // 점프 시 공중 상태 설정
}

void AHSBaseCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	bIsInAir = false; // 착지 시 공중 상태 해제
	UpdateMovementSpeed(); // 착지 시 속도 갱신
}

void AHSBaseCharacter::UpdateMovementSpeed()
{
	if (bIsInAir)
	{
		return;
	}

	float TargetSpeed = bIsRunning ? RunSpeed : WalkSpeed;
	GetCharacterMovement()->MaxWalkSpeed = TargetSpeed;
}

void AHSBaseCharacter::StartInteract()
{
}

void AHSBaseCharacter::ServerSetRunning_Implementation(bool bNewRunning)
{
	bIsRunning = bNewRunning;
	UpdateMovementSpeed();
}

bool AHSBaseCharacter::IsNetRelevantFor(const AActor* RealViewer, const AActor* Viewer, const FVector& SrcLocation) const
{
	// 자기 자신은 무조건 연관성 있음
	if (RealViewer == this || Viewer == this)
	{
		return true;
	}

	if (NetRelevancySightComponent)
	{
		return NetRelevancySightComponent->IsSeenBy(this, RealViewer, Viewer);
	}

	return Super::IsNetRelevantFor(RealViewer, Viewer, SrcLocation);
}