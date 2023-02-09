// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Vehicle.h"
#include "Kismet/GameplayStatics.h"
UMyGameInstance::UMyGameInstance()
{
	value = StateVehicle::MOVE;
	maxValue = 9;
}

void UMyGameInstance::ChangeModeVehicles()
{
	if (static_cast<int>(value) < maxValue - 1)
		value = static_cast<StateVehicle>(static_cast<int>(value) + 1);
	else value = StateVehicle::MOVE;
	
	if (GEngine) NameState(value);
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVehicle::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		AVehicle* vec = dynamic_cast<AVehicle*>(Actor);
		vec->ResetParameters();
	}
}