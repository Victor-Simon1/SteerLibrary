// Fill out your copyright notice in the Description page of Project Settings.

#include "StateVehicle.h"

void NameState(StateVehicle state)
{
	switch (state)
	{
	case StateVehicle::SEEK:
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,TEXT("SEEK"));
		break;
	case StateVehicle::FLEE:
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("FLEE"));
		break;
	case StateVehicle::PURSUE:
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("PURSUE"));
		break;
	case StateVehicle::EVADE:
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("EVADE"));
		break;
	case StateVehicle::ARRIVAL:
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("ARRIVAL"));
		break;
	case StateVehicle::MOVE:
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("MOVE"));
		break;
	default:
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("NULL"));
		break;
	}
}