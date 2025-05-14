// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/HSMissionInteractComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Interface/HSMissionTriggerInterface.h"
#include "Mission/HSMissionProgressTypes.h"
#include "Mission/HSMissionRow.h"
#include "Interface/HSMissionOwnerInterface.h"
#include "GameFramework/PlayerState.h"

// Sets default values for this component's properties
UHSMissionInteractComponent::UHSMissionInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	InteractBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractBox"));
	InteractBox->SetBoxExtent(FVector(60.f, 50.f, 80.f));
	InteractBox->SetCollisionProfileName(TEXT("InteractTrigger"));
	InteractBox->SetGenerateOverlapEvents(true);
	InteractBox->SetHiddenInGame(false);
}

void UHSMissionInteractComponent::RequestInteract()
{
	if (DetectedTargets.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("상호작용할 물체가 없습니다1."));
		return;
	}

	if (GetOwner()->HasAuthority())
	{
		PerformInteract();
	}
	else
	{
		ServerPerformInteract();
	}
}


void UHSMissionInteractComponent::OnRegister()
{
	Super::OnRegister();

	if (AActor* Owner = GetOwner())
	{
		if (USceneComponent* Root = Owner->GetRootComponent())
		{
			InteractBox->AttachToComponent(Root, FAttachmentTransformRules::SnapToTargetIncludingScale);
			InteractBox->SetRelativeLocation(FVector(50.f, 0.f, 0.f));
		}
	}
}

// Called when the game starts
void UHSMissionInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	InteractBox->OnComponentBeginOverlap.AddDynamic(this, &UHSMissionInteractComponent::OnInteractBoxBeginOverlap);
	InteractBox->OnComponentEndOverlap.AddDynamic(this, &UHSMissionInteractComponent::OnInteractBoxEndOverlap);
}

void UHSMissionInteractComponent::OnInteractBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->ImplementsInterface(UHSMissionTriggerInterface::StaticClass()))
	{
		IHSMissionTriggerInterface* Trigger = Cast<IHSMissionTriggerInterface>(OtherActor);
		IHSMissionOwnerInterface* MissionOwner = GetOwnerMissionInterface();
		if (Trigger && MissionOwner)
		{
			UE_LOG(LogTemp, Warning, TEXT("상호작용할 물체가 있습니다. %s"), *Trigger->GetTriggerMissionId().ToString());
			//if (MissionOwner->CanInteractMission(Trigger->GetTriggerMissionId()))
			if (MissionOwner->FindMissionProgressById(Trigger->GetTriggerMissionId()) != nullptr)
			{
				DetectedTargets.Add(OtherActor);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("상호작용할 수 없는 물체입니다.???"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("인터페이스 호출에 오류."));
		}
	}
}

void UHSMissionInteractComponent::OnInteractBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->GetClass()->ImplementsInterface(UHSMissionTriggerInterface::StaticClass()))
	{
		DetectedTargets.Remove(OtherActor);
	}
}

TScriptInterface<IHSMissionTriggerInterface> UHSMissionInteractComponent::FindNearestTarget()
{
	TScriptInterface<IHSMissionTriggerInterface> ClosestTarget;
	float MinDistSqr = MAX_FLT;
	const FVector MyLocation = GetOwner()->GetActorLocation();

	for (const TScriptInterface<IHSMissionTriggerInterface>& Target : DetectedTargets)
	{
		AActor* TargetActor = Cast<AActor>(Target.GetObject()); // 혹은 Target.GetObject()->GetActorLocation()
		IHSMissionTriggerInterface* TargetInterface = Target.GetInterface();
		if (!TargetActor || !TargetInterface) continue;

		float DistSqr = FVector::DistSquared(TargetActor->GetActorLocation(), MyLocation);
		if (DistSqr < MinDistSqr)
		{
			MinDistSqr = DistSqr;
			ClosestTarget = Target;
		}
	}

	return ClosestTarget;
}

void UHSMissionInteractComponent::PerformInteract()
{
	TScriptInterface<IHSMissionTriggerInterface> Target = FindNearestTarget();
	if (!Target)
	{
		UE_LOG(LogTemp, Warning, TEXT("상호작용 대상이 없습니다."));
		return;
	}

	// 트리거로부터 MissionId 받아오기
	const FName TargetMissionId = Target->GetTriggerMissionId();

	
	IHSMissionOwnerInterface* MissionOwner = GetOwnerMissionInterface();
	if (!MissionOwner) return;

	
	//if (!MissionOwner->CanInteractMission(TargetMissionId))
	if (MissionOwner->FindMissionProgressById(TargetMissionId) == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("일치하는 미션이 없습니다."));
		return;
	}

	MissionOwner->HandleMissionInteract(TargetMissionId);
}

IHSMissionOwnerInterface* UHSMissionInteractComponent::GetOwnerMissionInterface() const
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter)
	{
		return nullptr;
	}

	APlayerState* PlayerState = OwnerCharacter->GetPlayerState();
	if (!PlayerState)
	{
		return nullptr;
	}

	return Cast<IHSMissionOwnerInterface>(PlayerState);
}

void UHSMissionInteractComponent::ServerPerformInteract_Implementation()
{
	PerformInteract();
}



