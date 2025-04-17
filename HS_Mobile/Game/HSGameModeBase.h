// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HSGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class HS_MOBILE_API AHSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AHSGameModeBase();

	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
	TSubclassOf<class AHSSeekerCharacter> SeekerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
	TSubclassOf<class AHSBaseHiderCharacter> HiderClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
	FTransform SeekerSpawnTransform;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
	FTransform HiderSpawnTransform;

private:
	int32 PlayerCount = 0;
};
