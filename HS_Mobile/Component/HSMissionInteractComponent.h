// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HSMissionInteractComponent.generated.h"

// 전방 선언
class UBoxComponent;
class IHSMissionTriggerInterface;
class IHSMissionOwnerInterface;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HS_MOBILE_API UHSMissionInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHSMissionInteractComponent();

	void RequestInteract();

protected:
	// === 컴포넌트 생명주기 ===
	virtual void OnRegister() override;
	virtual void BeginPlay() override;

	// === 감지 영역 및 타겟 관리 ===
	UPROPERTY(VisibleAnywhere, Category = "Interact")
	TObjectPtr<UBoxComponent> InteractBox;

	UPROPERTY()
	TArray<TScriptInterface<IHSMissionTriggerInterface>> DetectedTargets;

	UFUNCTION()
	void OnInteractBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnInteractBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	TScriptInterface<IHSMissionTriggerInterface> FindNearestTarget();

	IHSMissionOwnerInterface* GetOwnerMissionInterface() const;

	void PerformInteract();

	UFUNCTION(Server, Reliable)
	void ServerPerformInteract();
		
};
