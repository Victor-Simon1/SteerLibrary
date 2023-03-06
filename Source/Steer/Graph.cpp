// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph.h"
#include "MyNode.h"
#include <map>
#include "Kismet/GameplayStatics.h"


Vertex::Vertex(AMyNode* n1, AMyNode* n2)
{
	this->n1 = n1;
	this->n2 = n2;
}
Vertex::~Vertex()
{

}
Graph::Graph()
{
	//InitGraph();
}

Graph::~Graph()
{
}


void Graph::InitGraph(UWorld* World)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, AMyNode::StaticClass(), FoundActors);
	for (int i = 0; i < FoundActors.Num(); i++)
	{
		listNode.push_back((AMyNode*)(FoundActors[i]));
	}
	for (int j = 0; j < listNode.size(); j++)
	{
		for(int k=0;k<listNode[j]->suiv.Num(); k++)
			listVertex.push_back(Vertex(listNode[j],listNode[j]->suiv[k]));
	}

}