// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <vector>
#include "MyNode.generated.h"

UCLASS(Blueprintable)
class STEER_API AMyNode : public AActor
{
	GENERATED_BODY()
	
public:	
	int id;
	std::vector<AMyNode> path;
	std::vector<int> suiv;
	// Sets default values for this actor's properties
	AMyNode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
