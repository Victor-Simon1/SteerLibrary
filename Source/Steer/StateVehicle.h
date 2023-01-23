// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <string>

UENUM()
enum class StateVehicle
{
	MOVE,
	SEEK,
	FLEE,
	PURSUE,
	EVADE,
	ARRIVAL,
	CIRCUIT,
	ONEWAY,
	TWOWAY
};

void NameState(StateVehicle state);
bool MovableState(StateVehicle state);