// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph.h"
#include "MyNode.h"
#include <map>
#include "Kismet/GameplayStatics.h"

Graph::Graph()
{
	//InitGraph();
}

Graph::~Graph()
{
}


void Graph::InitGraph(UWorld* World)
{
	/*TArray<AMyNode*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, AMyNode::StaticClass(), FoundActors);
	for (int i = 0; i < FoundActors.Num(); i++)
	{
		listNode.push_back((AMyNode)(*FoundActors[i]));
	}
	for (int j = 0; j < listNode.size(); j++)
	{
		for(int k=0;k<listNode[j].suiv.size();k++)
			listVertex.insert(std::pair<int, int>(listNode[j].id, listNode[j].suiv[k]));
	}*/

}