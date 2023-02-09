// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "StateVehicle.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class STEER_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

	

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	StateVehicle value;
	int maxValue;
	UMyGameInstance();

	UFUNCTION()
	void ChangeModeVehicles();
};
