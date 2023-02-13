// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Node.generated.h"

UCLASS()
class STEER_API ANode : public AActor
{
	GENERATED_BODY()
	
public:	

	int id;
	// Sets default values for this actor's properties
	ANode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
