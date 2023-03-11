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
	UPROPERTY(EditAnywhere)
	int id;
	//UPROPERTY(EditAnywhere)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float cost;
	AMyNode* parent;
	TArray<AMyNode *> path;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<AMyNode *> suiv;
	// Sets default values for this actor's properties
	AMyNode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
