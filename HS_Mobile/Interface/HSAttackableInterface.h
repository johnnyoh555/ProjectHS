// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HSAttackableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHSAttackableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HS_MOBILE_API IHSAttackableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetAsDead() = 0;
	virtual bool IsDead() const = 0;
};
