#include "HSNetRelevancySightComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"

UHSNetRelevancySightComponent::UHSNetRelevancySightComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	AngleThresholdCos = FMath::Cos(FMath::DegreesToRadians(MaxSightAngleDegrees));
}

bool UHSNetRelevancySightComponent::IsSeenBy(const AActor* TargetOwner, const AActor* RealViewer, const AActor* Viewer) const
{
	if (!TargetOwner || !Viewer)
	{
		return false;
	}

	// 기본값: Actor 기준
	FVector ViewerLocation = Viewer->GetActorLocation();
	FVector ViewDirection = Viewer->GetActorForwardVector();

	// 카메라 기준으로 보정
	if (const APawn* ViewerPawn = Cast<APawn>(Viewer))
	{
		if (const APlayerController* PC = Cast<APlayerController>(ViewerPawn->GetController()))
		{
			ViewDirection = PC->GetControlRotation().Vector();

			if (const APlayerCameraManager* CameraManager = PC->PlayerCameraManager)
			{
				ViewerLocation = CameraManager->GetCameraLocation();
			}
		}
	}

	const FVector TargetLocation = TargetOwner->GetActorLocation();

	// 거리 컷
	if (FVector::DistSquared(ViewerLocation, TargetLocation) > FMath::Square(MaxSightDistance))
	{
		return false;
	}

	// 시야각 컷
	const FVector ToTarget = (TargetLocation - ViewerLocation).GetSafeNormal();
	if (FVector::DotProduct(ViewDirection, ToTarget) < AngleThresholdCos)
	{
		return false;
	}

	// ------------------------------------------
	// 라인 트레이스: 앞/뒤/좌/우 4개 방향만 체크
	// ------------------------------------------
	TArray<FVector> Directions;
	Directions.Add(TargetLocation + FVector(Offset, 0, HeightOffset));   // 앞
	Directions.Add(TargetLocation + FVector(-Offset, 0, HeightOffset));   // 뒤
	Directions.Add(TargetLocation + FVector(0, Offset, HeightOffset));     // 오른쪽
	Directions.Add(TargetLocation + FVector(0, -Offset, HeightOffset));     // 왼쪽

	FCollisionQueryParams Params(SCENE_QUERY_STAT(NetRelevancySight), true, Viewer);
	bool bCanSee = false;

	for (const FVector& Point : Directions)
	{
		FHitResult Hit;
		const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, ViewerLocation, Point, ECC_Visibility, Params);

		if (!bHit || Hit.GetActor() == TargetOwner)
		{
			bCanSee = true;

			if (bDrawDebug)
			{
				DrawDebugLine(GetWorld(), ViewerLocation, Point, FColor::Green, false, 1.0f, 0, 1.0f);
			}
			break; // 하나라도 성공하면 바로 통과
		}

		if (bDrawDebug)
		{
			DrawDebugLine(GetWorld(), ViewerLocation, Point, FColor::Red, false, 1.0f, 0, 1.0f);
		}
	}

	// 디버그: 감지 지점 표시
	if (bDrawDebug)
	{
		DrawDebugSphere(GetWorld(), TargetLocation, 200.f, 12, bCanSee ? FColor::Green : FColor::Red, false, 1.0f);
	}

	return bCanSee;
}
