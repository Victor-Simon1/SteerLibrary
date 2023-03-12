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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector target;
	UPROPERTY(EditAnywhere)
	float max_force;
	UPROPERTY(EditAnywhere)
	float slowing_distance;
	std::vector<FVector> pathCircuit;
	std::vector<FVector> pathWay;
	std::vector<FVector> path2Way;
	int pathIndex;
	bool linkPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AMyNode* currentNode;
	int pathIndexWay;
	UPROPERTY(EditAnywhere)
	bool twoWay;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool finishSearch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AMyNode*> pathToAccomplish;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AMyNode*> seletectedNode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int mode;//0 : One point 1:Several point 2:Circuit
	int state;//enum
	UPROPERTY(EditAnywhere)
	bool finished;
	UMyGameInstance* GI; //= Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	APlayerController* player;
	// Sets default values for this actor's properties
	AVehicle();
	UFUNCTION(BlueprintCallable)
	void MoveTP1(float Delta);
	UFUNCTION(BlueprintCallable)
	void MoveTP2(float Delta);
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
	void OneWay2(float delta, TArray<AMyNode *> path);
	UFUNCTION(BlueprintCallable)
	void OnePoint(float delta);
	UFUNCTION(BlueprintCallable)
	void SeveralPoint(float delta);
	UFUNCTION(BlueprintCallable)
	void Circuit2(float delta);
	UFUNCTION(BlueprintCallable)
	void ChangeMode();
	void TwoWay(float delta);
	void ResetParameters();
	UFUNCTION(BlueprintCallable)
	TArray<AMyNode*> a_star(AMyNode *start, AMyNode *end);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void OnActionPressed();

};
