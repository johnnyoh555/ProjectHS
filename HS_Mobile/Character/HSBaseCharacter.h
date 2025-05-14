// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HSBaseCharacter.generated.h"

UCLASS(Abstract)
class HS_MOBILE_API AHSBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHSBaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 설정
protected:
	virtual void InitCapsule();
	virtual void InitMesh();
	void InitCamera();
	void InitInputMapping();
	void InitComponent();
	void SetCharacterStatData();

	// 캐릭터 동작 관련
protected:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Move(const struct FInputActionValue& Value);
	void Look(const struct FInputActionValue& Value);
	void ToggleRunning();
	void Jump() override;
	void Landed(const FHitResult& Hit) override;
	void UpdateMovementSpeed();
	// = 0 대신 이 매크로를 쓰면, 컴파일러에게 이 함수는 반드시 파생 클래스에서 구현해야 함을 알림
	virtual void StartInteract(); /* PURE_VIRTUAL(AHSBaseCharacter::StartInteract, );*/

	// 지금은 이렇게만 있어도 충분
	bool bIsRunning = false;
	bool bIsInAir = false;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	float WalkSpeed;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	float RunSpeed;

	// 캐릭터 스탯 관련
	UPROPERTY(EditDefaultsOnly, Category = "Stat")
	TObjectPtr<class UHSCharacterStatDataAsset> CharacterStatDataAsset;

	// 향상된 입력 시스템용
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> InputMapping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> RunAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> InteractAction;

	// 서버와 클라이언트 RPC 함수 선언
	UFUNCTION(Server, Unreliable)
	void ServerSetRunning(bool bNewRunning);

	// 카메라 관련
	// 카메라 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NetRelevancy", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UHSNetRelevancySightComponent> NetRelevancySightComponent;

	virtual bool IsNetRelevantFor(const AActor* RealViewer, const AActor* Viewer, const FVector& SrcLocation) const override;

};
