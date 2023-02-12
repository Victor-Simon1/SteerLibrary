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
	UPROPERTY(EditAnywhere)
	float max_speed;
	UPROPERTY(EditAnywhere)
	float mass;
	UPROPERTY(EditAnywhere)
	FVector velocity;
	FMatrix orientation;
	FQuat quat;
	UPROPERTY(EditAnywhere)
	FVector target;
	UPROPERTY(EditAnywhere)
	float max_force;
	UPROPERTY(EditAnywhere)
	float slowing_distance;
	std::vector<FVector> pathCircuit;
	std::vector<FVector> pathWay;
	std::vector<FVector> path2Way;
	int pathIndex;
	int pathIndexWay;
	UPROPERTY(EditAnywhere)
	bool twoWay;
	int state;//enum
	UPROPERTY(EditAnywhere)
	bool finished;
	UMyGameInstance* GI; //= Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	APlayerController* player;
	// Sets default values for this actor's properties
	AVehicle();
	void SetOrientation();
	void ChangeVelocity(float deltaTime,FVector steering);
	void Seek(float delta);
	void Flee(float delta);
	void Pursue(float delta);
	void Evade(float delta);
	void Arrival(float delta);
	void Move(float delta);
	void Circuit(float delta);
	void OneWay(float delta,std::vector<FVector> path);
	void TwoWay(float delta);
	void ResetParameters();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void OnActionPressed();

};
