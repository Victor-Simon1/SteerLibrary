// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNode.h"

// Sets default values
AMyNode::AMyNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	cost = 0;
//this->id = 0;
}
/*
AMyNode& AMyNode::operator=(const AMyNode& n)
{
	id = n.id;
	cost = n.cost;
	parent = n.parent;
	return *this;
}*/

// Called when the game starts or when spawned
void AMyNode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

