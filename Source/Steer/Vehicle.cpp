// Fill out your copyright notice in the Description page of Project Settings.
#include "Vehicle.h"
#include "Math/Vector.h"
#include <iostream>
#include "Kismet/GameplayStatics.h"

// Sets default values
AVehicle::AVehicle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	max_speed = 10.0;
	max_force = 10.0;
	mass = 10.0;
	velocity = FVector();
	state = StateVehicle::MOVE;
}
void AVehicle::Move()
{
	if (FVector::Dist(this->target, this->GetActorLocation())<1)
	{
		this->target = FVector(rand() % 2000, rand() % 200, 0);
	}
	FQuat rot;
	SetActorLocationAndRotation(this->target,rot , false, 0, ETeleportType::None);
}
void AVehicle::Seek()
{
	FQuat rot;
	FVector temp = this->target - this->GetActorLocation();
	FVector desired_velocity = (FVector)temp.Normalize() * max_speed;
	FVector steering = desired_velocity - velocity;
	SetActorLocationAndRotation(steering, rot, false, 0, ETeleportType::None);
}
void AVehicle::Flee()
{
	FQuat rot;
	FVector temp = this->target - this->GetActorLocation();
	FVector desired_velocity = (FVector)temp.Normalize() * max_speed;
	FVector steering = -desired_velocity - velocity;
	SetActorLocationAndRotation(steering, rot, false, 0, ETeleportType::None);
}
void AVehicle::Pursue()
{
	FQuat rot;
	FVector temp = this->target - this->GetActorLocation();
	FVector desired_velocity = (FVector)temp.Normalize() * max_speed;
	FVector steering = desired_velocity - velocity;
	SetActorLocationAndRotation(steering, rot, false, 0, ETeleportType::None);
}

void AVehicle::Evade()
{
	FQuat rot;
	FVector temp = this->target - this->GetActorLocation();
	FVector desired_velocity = (FVector)temp.Normalize() * max_speed;
	FVector steering = desired_velocity - velocity;
	SetActorLocationAndRotation(steering, rot, false, 0, ETeleportType::None);
}

void AVehicle::Arrival()
{
	FQuat rot;
	FVector target_offset = this->target - this->GetActorLocation();
	float distance = (target_offset.X+ target_offset.Y+ target_offset.Z) / 3;
	float ramped_speed = max_speed * (distance / slowing_distance);
	float clipped_speed = std::min(ramped_speed, max_speed);
	FVector desired_velocity = (clipped_speed / distance) * target_offset;
	FVector steering = desired_velocity - velocity;
	SetActorLocationAndRotation(steering, rot, false, 0, ETeleportType::None);
}


// Called when the game starts or when spawned
void AVehicle::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* FirstLocalPlayer = UGameplayStatics::GetPlayerController(this, 0);

	// Ensure that the PlayerController and its InputComponent are valid before binding
	if (IsValid(FirstLocalPlayer) && IsValid(FirstLocalPlayer->InputComponent))
	{
		// Use BindAction or BindAxis on the InputComponent to establish the bindings
		FirstLocalPlayer->InputComponent->BindAction(FName("MyAction"), IE_Pressed, this, &AVehicle::OnActionPressed);
	}
	this->target = FVector(100.0, 100.0, 0.0);
	std::cout << "Init Vehicle" << std::endl;
}
void AVehicle::OnActionPressed()
{
	UE_LOG(LogTemp, Log, TEXT("Key Pressed"));
}
// Called every frame
void AVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//FVector target;

	switch (state)
	{
	case StateVehicle::SEEK:
		Seek();
		break;
	case StateVehicle::FLEE:
		Flee();
		break;
	case StateVehicle::PURSUE:
		Pursue();
		break;
	default:
		Move();
	}

}

