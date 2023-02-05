// Fill out your copyright notice in the Description page of Project Settings.
#include "Vehicle.h"
#include "Math/Vector.h"
#include "Math/UnrealMathUtility.h"
#include <iostream>
#include <algorithm>
#include "MyPawn.h"
#include "ActorCircuit.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AVehicle::AVehicle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	max_speed = 250.0;
	max_force = 30.0;
	mass = 5.0;
	slowing_distance = 25.0;
	velocity = FVector(1.0,1.0,0.0);
	pathIndex = 0;
	//acceleration = 1.0;
	//state = 0; //StateVehicle::MOVE;
	
}
FVector Truncate(FVector v, float m)
{
	if (v.Length() == 0)return v;
	return (m * v) / v.Length();
}
void AVehicle::Move(float delta)
{
	float acceleration = 1.0;
	//UE_LOG(LogTemp, Warning, TEXT(FVector::Dist(this->target, this->GetActorLocation())));
	if (FVector::Dist(this->target, this->GetActorLocation())<50)
	{
		UE_LOG(LogTemp, Warning, TEXT("New Target Vehicle"));
		this->target = FVector(rand() % 2000, rand() % 200, 100.0);
		//state = rand() % 4;
	}
	FQuat rot;
	FVector currentLocation = this->GetActorLocation();
	FVector newLocation;
	if (abs(currentLocation.X - this->target.X) > 20 && abs(currentLocation.Y - this->target.Y) > 20) {
		if (abs(currentLocation.X - this->target.X) >= 20) {
			if ((this->target.X - currentLocation.X) < 0)
				velocity.X -= acceleration;
			else
				velocity.X += acceleration;
		}

		if (abs(currentLocation.Y - this->target.Y) >= 20) {
			if ((this->target.Y - currentLocation.Y) < 0)
				velocity.Y -= acceleration;
			else
				velocity.Y += acceleration;
		}
	}
	if (velocity.X > max_speed)
		velocity.X = max_speed;
	else if (velocity.X < -max_speed)
		velocity.X = -max_speed;
	if (velocity.Y > max_speed)
		velocity.Y = max_speed;
	else if (velocity.Y < -max_speed)
		velocity.Y = -max_speed;

	newLocation = currentLocation + velocity * delta;
	newLocation.Z = 100.0;
	SetActorLocation(newLocation);
	//SetActorLocationAndRotation(this->target,rot , false, 0, ETeleportType::None);
}
void AVehicle::Seek(float delta)
{
	FVector newLocation;
	FVector currentLocation = this->GetActorLocation();
	FVector temp;
	//UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if(!MovableState(GI->value))
		temp = player->GetPawn()->GetActorLocation() - currentLocation;
	else temp = target - currentLocation;

	FVector desired_velocity = temp * max_speed;
	FVector steering = desired_velocity - velocity;

	FVector steering_force = Truncate(steering, max_force);

	FVector acceleration = steering_force / mass;

	velocity = Truncate(velocity+acceleration, max_speed);

	newLocation = currentLocation + velocity * delta;
	SetActorLocation(newLocation);
	
}
void AVehicle::Flee(float delta)
{
	FVector newLocation;
	FVector currentLocation = this->GetActorLocation();
	FVector temp = player->GetPawn()->GetActorLocation() - currentLocation;

	FVector desired_velocity = temp * max_speed;
	FVector steering = -desired_velocity - velocity;

	FVector steering_force = Truncate(steering, max_force);

	FVector acceleration = steering_force / mass;

	velocity = Truncate(velocity + acceleration, max_speed);
	newLocation = currentLocation + velocity * delta;
	SetActorLocation(newLocation);
}
void AVehicle::Pursue(float delta)
{
	FVector newLocation;
	FVector currentLocation = this->GetActorLocation();

	FVector dist = player->GetPawn()->GetActorLocation() - currentLocation ;
	/*
	float d = FVector::DotProduct(dynamic_cast<AMyPawn*>(player->GetPawn())->velocity,dist);
	FVector temp = velocity;
	FVector temp2 = dynamic_cast<AMyPawn *>(player->GetPawn())->velocity;
	temp.Normalize(); 
	temp2.Normalize();
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Velo %s"), *temp2.ToString()));
	float c = FVector::DotProduct(temp,temp2 );

	dist = player->GetPawn()->GetActorLocation() * (d * c) - currentLocation;*/
	FVector desired_velocity = dist*3.0 * max_speed;
	FVector steering = desired_velocity - velocity;
	FVector steering_force = Truncate(steering, max_force);

	FVector acceleration = steering_force / mass;

	velocity = Truncate(velocity + acceleration, max_speed);

	newLocation = currentLocation + velocity * delta;
	SetActorLocation(newLocation);
}

void AVehicle::Evade(float delta)
{
	FVector newLocation;
	FVector currentLocation = this->GetActorLocation();
	FVector dist = player->GetPawn()->GetActorLocation() - currentLocation;

	FVector desired_velocity = dist * 3.0 * max_speed;
	FVector steering = -desired_velocity - velocity;
	FVector steering_force = Truncate(steering, max_force);

	FVector acceleration = steering_force / mass;
	
	velocity = Truncate(velocity + acceleration, max_speed);

	newLocation = currentLocation + velocity * delta;
	SetActorLocation(newLocation);
}

void AVehicle::Arrival(float delta)
{
	FVector newLocation;
	FVector currentLocation = this->GetActorLocation();
	FVector target_offset;
	if (!MovableState(GI->value))
		target_offset = player->GetPawn()->GetActorLocation() - currentLocation;
	else target_offset = target - currentLocation;
	//FVector target_offset = player->GetPawn()->GetActorLocation() - currentLocation;
	float distance = target_offset.Length();
	float ramped_speed = max_speed * (distance / slowing_distance);
	float clipped_speed = std::min(ramped_speed, max_speed);

	FVector desired_velocity = (clipped_speed / distance) * target_offset;
	FVector steering = desired_velocity - velocity;
	FVector steering_force = Truncate(steering, max_force);
	FVector acceleration = steering_force / mass;

	if ((velocity + acceleration).Length() > max_speed)
		velocity = Truncate(velocity + acceleration, max_speed);
	else velocity = velocity + acceleration;

	newLocation = currentLocation + velocity * delta;
	SetActorLocation(newLocation);
}

void AVehicle::Circuit(float deltaTime)
{
	if (pathCircuit.empty())
	{
		int nbPoint = 4;//rand() % 25;
		float phiAdd = 360 / nbPoint;
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Velo %f"), phiAdd));
		float phi = 0;
		float r = 500.0;
		for (int i = 0; i < nbPoint; i++)
		{
			pathCircuit.push_back(FVector( r*cos(phi*(PI /180)), r*sin(phi * (PI / 180)), 100));
			FRotator Rotation(0.0f, 0.0f, 0.0f);
			FActorSpawnParameters SpawnInfo;
			AActorCircuit* mySphere = GetWorld()->SpawnActor<AActorCircuit>(pathCircuit.back(),Rotation,SpawnInfo);
			phi += phiAdd;
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("phi %f"), phi));
		}
		//path.push_back(path[0]);
	}
	if ((this->target - this->GetActorLocation()).Length() < 50)
	{
		if (pathIndex >= pathCircuit.size())pathIndex = 0;
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("phi %d"), pathIndex));
		this->target = pathCircuit[pathIndex];
		pathIndex++;
	}
	Seek(deltaTime);
}
void AVehicle::OneWay(float deltaTime)
{
	if (pathWay.empty())
	{
		int nbPoint = 8;
		for (int i = 0; i < nbPoint; i++)
		{
			pathWay.push_back(FVector((float)i * 200, 0, 0));
			FRotator Rotation(0.0f, 0.0f, 0.0f);
			FActorSpawnParameters SpawnInfo;
			AActorCircuit* mySphere = GetWorld()->SpawnActor<AActorCircuit>(pathWay.back(), Rotation, SpawnInfo);
		}
	}
	if ((this->target - this->GetActorLocation()).Length() < 50)
	{
		this->target = pathWay[pathIndexWay];
		pathIndexWay++;
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("phi %d"), pathIndexWay));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("size %d"), pathWay.size()));
	}
	if (pathIndexWay == pathWay.size())
		Arrival(deltaTime);
	else if(pathIndexWay < pathWay.size())
		Seek(deltaTime);
}
void AVehicle::TwoWay(float deltaTime)
{
	if (pathWay.empty())
	{
		int nbPoint = 8;
		for (int i = 0; i < nbPoint; i++)
		{
			pathWay.push_back(FVector((float)i * 200, 0, 0));
			FRotator Rotation(0.0f, 0.0f, 0.0f);
			FActorSpawnParameters SpawnInfo;
			AActorCircuit* mySphere = GetWorld()->SpawnActor<AActorCircuit>(pathWay.back(), Rotation, SpawnInfo);
		}
	}
	if ((this->target - this->GetActorLocation()).Length() < 50)
	{
		this->target = pathWay[pathIndexWay];
		pathIndexWay++;
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("phi %d"), pathIndexWay));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("size %d"), pathWay.size()));
		if (pathIndexWay == pathWay.size() + 1)
		{
			std::reverse(pathWay.begin(), pathWay.end());
			pathIndex = 0;
		}
	}
	if (pathIndexWay == pathWay.size())
		Arrival(deltaTime);
	else if (pathIndexWay < pathWay.size())
		Seek(deltaTime);
	
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
// Called every frame
void AVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
		OneWay(DeltaTime);
		break;
	case StateVehicle::TWOWAY:
		TwoWay(DeltaTime);
		break;
	}
}

