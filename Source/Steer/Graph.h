// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Node.h"
#include <vector>
#include <map>
/**
 * 
 */
class STEER_API Graph
{
public:
	std::vector<ANode> listNode;
	std::multimap<int, std::vector<ANode>> listVertex;
	int nbNode;
	Graph();
	~Graph();
	void InitGraph(UWorld* World);
	

};
