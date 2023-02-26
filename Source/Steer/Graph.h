// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyNode.h"
#include <vector>
#include <map>
/**
 * 
 */
class STEER_API Graph
{
public:
	std::vector<AMyNode> listNode;
	std::multimap<int, int> listVertex;
	int nbNode;
	Graph();
	~Graph();
	void InitGraph(UWorld* World);
	

};
