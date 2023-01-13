// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Vehicle.generated.h"


enum class StateVehicle
{
	SEEK,
	FLEE,
	PURSUE
};
UCLASS()
class STEER_API AVehicle : public AActor
{
	GENERATED_BODY()
	
public:

	float max_speed;
	float mass;
	FVector velocity;
	float max_force;
	StateVehicle state;
	// Sets default values for this actor's properties
	AVehicle();

	void Seek(const FVector& target);
	void Flee(const FVector& target);
	void Pursue(const FVector& target);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void OnActionPressed();

};
