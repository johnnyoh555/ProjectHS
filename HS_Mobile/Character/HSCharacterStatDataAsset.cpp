// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HSCharacterStatDataAsset.h"
#include "HSCharacterStatDataAsset.h"

UHSCharacterStatDataAsset::UHSCharacterStatDataAsset()
{
	TargetArmLength = 400.0f;
	BrakingDecelerationWalking = 2000.f;
	JumpZVelocity = 700.f;
	AirControl = 0.35f;
	MaxWalkSpeed = 200.0f;
	RunSpeed = 500.0f;
	WalkSpeed = 250.0f;
	MinAnalogWalkSpeed = 20.f;
	BrakingDecelerationWalking = 2000.f;
	GravityScale = 1.0f;
}