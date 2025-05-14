// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Interface/HSMissionOwnerInterface.h"
#include "HSPlayerState.generated.h"


UENUM(BlueprintType)
enum class EPlayerRole : uint8
{
	Hider,
	Seeker
};

class UHSMissionBase;
struct FMissionProgress;
/**
 * 
 */
UCLASS()
class HS_MOBILE_API AHSPlayerState : public APlayerState, public IHSMissionOwnerInterface
{
	GENERATED_BODY()
	
public:
	AHSPlayerState();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mission", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UHSPlayerMissionManagerComponent> MissionManager;

	UPROPERTY(Replicated, BlueprintReadOnly)
	EPlayerRole PlayerRole = EPlayerRole::Hider; // 기본은 도망자로

	

public:
	FORCEINLINE void SetPlayerRole(EPlayerRole NewRole) { PlayerRole = NewRole; }

	void AddMission(UHSMissionBase* NewMission);

	// 미션 진행 상태 조회
	virtual struct FMissionProgress* FindMissionProgressById(const FName& MissionId);

	// 상호작용 가능한지 검사
	virtual bool CanInteractMission(const FName& MissionId) const;

	virtual void HandleMissionInteract(const FName& MissionId) override;
};
