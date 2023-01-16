// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Vehicle.generated.h"


enum class StateVehicle
{
	MOVE,
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
	float vx, vy;
	float acceleration;
	FVector velocity;
	UPROPERTY(EditAnywhere)
	FVector target;
	float max_force;
	float slowing_distance;
	StateVehicle state;
	// Sets default values for this actor's properties
	AVehicle();

	void Seek();
	void Flee();
	void Pursue();
	void Evade();
	void Arrival();
	void Move(float delta);
	void BindToInput();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void OnActionPressed();

};
