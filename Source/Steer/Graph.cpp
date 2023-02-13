// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph.h"
#include "Node.h"
#include "Kismet/GameplayStatics.h"

Graph::Graph()
{
}

Graph::~Graph()
{
}


void Graph::InitGraph(UWorld* World)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, ANode::StaticClass(), FoundActors);


}