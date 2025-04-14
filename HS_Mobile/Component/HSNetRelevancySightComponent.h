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

	bool IsSeenBy(USphereComponent* NetRelevancySphere, const AActor* RealViewer, const AActor* Viewer) const;
};
