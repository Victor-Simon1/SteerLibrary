// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyGameInstance.h"
#include "Vehicle.generated.h"



UCLASS()
class STEER_API AVehicle : public AActor
{
	GENERATED_BODY()
	
public:

	float max_speed;
	float mass;
	UPROPERTY(EditAnywhere)
	FVector velocity;
	UPROPERTY(EditAnywhere)
	FVector target;
	float max_force;
	float slowing_distance;
	std::vector<FVector> pathCircuit;
	std::vector<FVector> pathWay;
	int pathIndex;
	int pathIndexWay;
	int pathIndex2Way;
	bool twoWay;
	int state;//enum
	UMyGameInstance* GI; //= Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	APlayerController* player;
	// Sets default values for this actor's properties
	AVehicle();

	void Seek(float delta);
	void Flee(float delta);
	void Pursue(float delta);
	void Evade(float delta);
	void Arrival(float delta);
	void Move(float delta);
	void Circuit(float delta);
	void OneWay(float delta);
	void TwoWay(float delta);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void OnActionPressed();

};
