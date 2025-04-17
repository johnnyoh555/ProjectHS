// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "HSNetRelevancySightComponent.generated.h"

/**
 * UNetRelevancySightComponent
 *
 * This component is used to determine if an actor is within the sight of a player controller.
 * It uses a sphere component to check if the actor is within a certain radius and angle from the player's camera.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HS_MOBILE_API UHSNetRelevancySightComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHSNetRelevancySightComponent();

	/** 시야 내에서 대상 영역이 감지되는지 여부를 반환 */
	bool IsSeenBy(const AActor* TargetOwner, const AActor* RealViewer, const AActor* Viewer) const;

protected:
	UPROPERTY(EditAnywhere, Category = "Sight")
	float MaxSightDistance = 3000.0f;

	UPROPERTY(EditAnywhere, Category = "Sight")
	float MaxSightAngleDegrees = 55.0f;

	float AngleThresholdCos = 0.f;

	UPROPERTY(EditAnywhere, Category = "Sight")
	float Offset = 200.f; // 오프셋 거리
	
	UPROPERTY(EditAnywhere, Category = "Sight")
	float HeightOffset = 30.f; // 높이 오프셋

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDrawDebug = true;
};