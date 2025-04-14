// Fill out your copyright notice in the Description page of Project Settings.


#include "HSNetRelevancySightComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h" // 꼭 필요!

UHSNetRelevancySightComponent::UHSNetRelevancySightComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UHSNetRelevancySightComponent::IsSeenBy(USphereComponent* NetRelevancySphere, const AActor* RealViewer, const AActor* Viewer) const
{
	if (!NetRelevancySphere || !Viewer) return false;

	const FVector TargetLocation = NetRelevancySphere->GetComponentLocation();
	const FVector ViewerLocation = Viewer->GetActorLocation();

	// 거리 기반 컷
	constexpr float MaxSightDistance = 2000.0f;
	if (FVector::DistSquared(ViewerLocation, TargetLocation) > FMath::Square(MaxSightDistance))
	{
		return false;
	}

	 // 시야각 기반 컷 (잠시 주석처리해도 됨)
	 const FVector ToTarget = (TargetLocation - ViewerLocation).GetSafeNormal();
	 const FVector ViewDirection = Viewer->GetActorForwardVector();
	 if (FVector::DotProduct(ToTarget, ViewDirection) < FMath::Cos(FMath::DegreesToRadians(75.0f)))
	 {
	     return false;
	 }

	// 실제 라인 오브 사이트 체크
	if (const AController* Controller = Cast<AController>(Viewer))
	{
		return Controller->LineOfSightTo(NetRelevancySphere->GetOwner());
	}

	// Controller가 아니라면 일단 기본적으로 true 반환 (디버깅용)
	return true;
}