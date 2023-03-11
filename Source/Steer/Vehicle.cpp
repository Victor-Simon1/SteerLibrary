// Fill out your copyright notice in the Description page of Project Settings.
#include "Vehicle.h"
#include "Math/Vector.h"
#include <vector>
#include "Math/UnrealMathUtility.h"
#include <iostream>
#include <algorithm>
#include "MyPawn.h"
#include "Graph.h"
#include "MyNode.h"
#include "ActorCircuit.h"
#include "Kismet/GameplayStatics.h"
#include <regex>

// Sets default values
AVehicle::AVehicle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	max_speed = 210.0;
	max_force = 40.0;
	mass = 15.0;
	slowing_distance = 100.0;
	velocity = FVector(1.0,1.0,0.0);
	ResetParameters();
}

void AVehicle::ResetParameters()
{
	pathIndex = 0;
	pathIndexWay = 0;
	finished = false;
	twoWay = false;
	//if(!pathWay.empty())std::reverse(pathWay.begin(), pathWay.end());
}
FVector Truncate(FVector v, float m)
{
	if (v.Length() == 0)return v;
	return (m * v) / v.Length();
}

void AVehicle::SetOrientation()
{
	FVector new_forward = velocity;
	new_forward.Normalize();
	SetActorRotation(new_forward.Rotation());

}
void AVehicle::Move(float delta)
{

	if (FVector::Dist(this->target, this->GetActorLocation())<50)
	{
	//	UE_LOG(LogTemp, Warning, TEXT("New Target Vehicle"));
		this->target = FVector(rand() % 2000, rand() % 200, 100.0);
	}

	FVector newLocation;
	FVector currentLocation = this->GetActorLocation();
	FVector temp = target - currentLocation;

	FVector desired_velocity = temp * max_speed;
	FVector steering = desired_velocity - velocity;

	FVector steering_force = Truncate(steering, max_force);

	FVector acceleration = steering_force / mass;

	velocity = Truncate(velocity + acceleration, max_speed);
	newLocation = currentLocation + velocity * delta;
	newLocation.Z = 100.0;
	SetActorLocation(newLocation);
	SetOrientation();
}

void AVehicle::ChangeVelocity(float deltaTime,FVector steering)
{
	FVector newLocation;
	FVector currentLocation = this->GetActorLocation();
	FVector steering_force = Truncate(steering, max_force);

	FVector acceleration = steering_force / mass;
	if ((velocity + acceleration).Length() > max_speed)
		velocity = Truncate(velocity + acceleration, max_speed);
	else velocity = velocity + acceleration;
	newLocation = currentLocation + velocity * deltaTime;
	SetActorLocation(newLocation);
	SetOrientation();
}

void AVehicle::Seek(float delta)
{
	FVector currentLocation = this->GetActorLocation();
	FVector temp;
	if(!MovableState(GI->value))
		temp = player->GetPawn()->GetActorLocation() - currentLocation;
	else temp = target - currentLocation;
	
	FVector desired_velocity = temp * max_speed;
	FVector steering = desired_velocity - velocity;
	ChangeVelocity(delta, steering);
}

void AVehicle::Flee(float delta)
{
	FVector currentLocation = this->GetActorLocation();
	FVector temp = player->GetPawn()->GetActorLocation() - currentLocation;

	FVector desired_velocity = temp * max_speed;
	FVector steering = -desired_velocity - velocity;
	ChangeVelocity(delta, steering);

}
void AVehicle::Pursue(float delta)
{
	FVector currentLocation = this->GetActorLocation();

	FVector dist = player->GetPawn()->GetActorLocation() - currentLocation ;
	FVector desired_velocity = dist*3.0 * max_speed;
	FVector steering = desired_velocity - velocity;
	ChangeVelocity(delta, steering);

}

void AVehicle::Evade(float delta)
{
	FVector currentLocation = this->GetActorLocation();
	FVector dist = player->GetPawn()->GetActorLocation() - currentLocation;

	FVector desired_velocity = dist * 3.0 * max_speed;
	FVector steering = -desired_velocity - velocity;
	ChangeVelocity(delta, steering);

}

void AVehicle::Arrival(float delta)
{
	FVector currentLocation = this->GetActorLocation();
	FVector target_offset;
	if (!MovableState(GI->value))
		target_offset = player->GetPawn()->GetActorLocation() - currentLocation;
	else target_offset = target - currentLocation;
	//FVector target_offset = player->GetPawn()->GetActorLocation() - currentLocation;
	float distance = target_offset.Length();
	float ramped_speed = max_speed * (distance / slowing_distance);
	float clipped_speed = std::min(ramped_speed, max_speed);
;
	FVector desired_velocity = (clipped_speed / distance) * target_offset;
	FVector steering = desired_velocity - velocity;
	ChangeVelocity(delta, steering);
}

void AVehicle::Circuit(float deltaTime)
{
	if (pathCircuit.empty())
	{
		int nbPoint =  10;
		float phiAdd = 360 / nbPoint;
		float phi = 0;
		float r = 800.0;
		for (int i = 0; i < nbPoint; i++)
		{
			pathCircuit.push_back(FVector( r*cos(phi*(PI /180)), r*sin(phi * (PI / 180)), 100.0));
			FRotator Rotation(0.0f, 0.0f, 0.0f);
			FActorSpawnParameters SpawnInfo;
			AActorCircuit* mySphere = GetWorld()->SpawnActor<AActorCircuit>(pathCircuit.back(),Rotation,SpawnInfo);
			phi += phiAdd;
		}
	}
	if ((this->target - this->GetActorLocation()).Length() < 50)
	{
		if (pathIndex >= pathCircuit.size())pathIndex = 0;
		this->target = pathCircuit[pathIndex];
		pathIndex++;
	}
	Seek(deltaTime);
}
void AVehicle::OneWay(float deltaTime,std::vector<FVector> path)
{
	if (pathWay.empty())
	{
		int nbPoint = 8;
		for (int i = 0; i < nbPoint; i++)
		{
			pathWay.push_back(FVector((float)i * 500, 1500+rand()%150, 0));
			path2Way.push_back(pathWay.back());
			FRotator Rotation(0.0f, 0.0f, 0.0f);
			FActorSpawnParameters SpawnInfo;
			AActorCircuit* mySphere = GetWorld()->SpawnActor<AActorCircuit>(pathWay.back(), Rotation, SpawnInfo);
		}
		std::vector<FVector> temp = pathWay;
		std::reverse(temp.begin(), temp.end());
		path2Way.insert(path2Way.end(), temp.begin(), temp.end());
	}
	if ((this->target - this->GetActorLocation()).Length() < 50 && !(pathIndexWay == path.size()))
	{
		this->target = path[pathIndexWay];
		pathIndexWay++;
	}
	if (pathIndexWay == path.size())
	{
		Arrival(deltaTime);
		if((this->target - this->GetActorLocation()).Length() < 5)
			pathIndexWay++;
	}
	else if (pathIndexWay < path.size())
		Seek(deltaTime);
	else finished = true;
}
void AVehicle::TwoWay(float deltaTime)
{

	if (!twoWay)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("first way"));
	//	OneWay(deltaTime);
		if (finished)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("second way"));
			twoWay = true;
			pathIndexWay = 0;
			//std::reverse(pathWay.begin(), pathWay.end());
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("second way"));
	//	OneWay(deltaTime);
	}
}
// Called when the game starts or when spawned
void AVehicle::BeginPlay()
{
	Super::BeginPlay();
	this->target = FVector(100.0, 100.0, 100.0);
	GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	player = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UE_LOG(LogTemp, Warning, TEXT("Init Vehicle"));
}

void AVehicle::OnActionPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Key Pressed"));
}

void AVehicle::MoveTP1(float DeltaTime)
{
	if (!MovableState(GI->value)) this->target = player->GetPawn()->GetActorLocation();
	switch (GI->value)
	{
	case StateVehicle::SEEK:
		Seek(DeltaTime);
		break;
	case StateVehicle::FLEE:
		Flee(DeltaTime);
		break;
	case StateVehicle::PURSUE:
		Pursue(DeltaTime);
		break;
	case StateVehicle::EVADE:
		Evade(DeltaTime);
		break;
	case StateVehicle::ARRIVAL:
		Arrival(DeltaTime);
		break;
	case StateVehicle::MOVE:
		Move(DeltaTime);
		break;
	case StateVehicle::CIRCUIT:
		Circuit(DeltaTime);
		break;
	case StateVehicle::ONEWAY:
		OneWay(DeltaTime, pathWay);
		break;
	case StateVehicle::TWOWAY:
		OneWay(DeltaTime, path2Way);
		break;
	}
}

void AVehicle::MoveTP2()
{

}
// Called every frame
void AVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

template <class T>
inline bool include_in_list(std::vector<T> list, T &elem)
{
	return std::find(list.begin(), list.end(), elem) != list.end();
}
inline bool est_but(AMyNode *node, AMyNode *end)
{
	return node == end;
}
AMyNode* lowest_cost(TArray<AMyNode *> list,int &pos)
{
	AMyNode* minNode = list[0];
	pos = 0;
	for (int i = 1; i < list.Num(); i++) 
	{
		if (minNode->cost > list[i]->cost)
		{
			minNode = list[i];
			pos = i;
		}
	}
	return minNode;
}
TArray<AMyNode *> AVehicle::a_star(AMyNode* start, AMyNode *end) {
	TArray<AMyNode *> open;
	TArray<AMyNode *> closed;
	AMyNode* current = nullptr;
	open.Add(start);
	//current = start;
	// int nb = 1;
	int pos = 0;
	while (!open.IsEmpty()){
		current = lowest_cost(open,pos);
		open.RemoveAt(pos);
		
		if (est_but(current, end)) {//path trouv�
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,TEXT("SEEK"));
			break;
		}
		closed.Add(current);
		for (AMyNode *fils : current->suiv)
		{
			//if (!(std::find(closed.begin(), closed.end(), fils) != closed.end()))// n'appartient pas � ancien
			//{
				//ajout du cout du voisin
			if(!closed.Contains(fils)){
				fils->cost = current->cost + FVector::Dist(fils->GetActorLocation(),current->GetActorLocation());
				//if (current->cost + < current->cost)
			//	{
					fils->path.Append(current->path);
					fils->path.Add(current);
					//if(!l.Contains(fils))
					open.Add(fils);
			//	}
				//ajout de parent � current
				//vert.n2->parent = vert.n1; 
				//si voisin n'et pas dans l
				//if (std::find(l.begin(), l.end(), vert.n2->id) != l.end())
				//l.Add(vert.n2);
				//l.push_back(findNodeById(itr->second));	
				
			}
		}
		
	}
	current->path.Add(current);
	return current->path;
}


