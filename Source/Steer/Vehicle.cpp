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

	max_speed = 150.0;
	max_force = 10.0;
	vx = 0;
	vy = 0;
	mass = 50.0;
	velocity = FVector(1.0,1.0,0.0);
	acceleration = 1.0;
	state = 0; //StateVehicle::MOVE;
	
	
}
void AVehicle::Move(float delta)
{
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

	newLocation.X = currentLocation.X + velocity.X * delta;
	newLocation.Y = currentLocation.Y + velocity.Y * delta;
	newLocation.Z = 100.0;
	SetActorLocation(newLocation);
	//SetActorLocationAndRotation(this->target,rot , false, 0, ETeleportType::None);
}
void AVehicle::Seek(float delta)
{
	FVector newLocation;
	FVector currentLocation = this->GetActorLocation();
	FVector temp = player->GetPawn()->GetActorLocation() - currentLocation;

	if (velocity.X + acceleration < max_speed)
	{
		velocity.X = velocity.X + acceleration;
		velocity.Y = velocity.Y + acceleration;
	}
	
	FVector desired_velocity = temp * max_speed;
	FVector steering = desired_velocity - velocity;

	if (steering.X > max_force)steering.X = max_force;
	if (steering.Y > max_force)steering.Y = max_force;
	steering /= mass;

	if (velocity.X + steering.X > max_speed)
		velocity.X = max_speed;
	else if(velocity.X + steering.X < -max_speed)
		velocity.X = -max_speed;
	if (velocity.Y + steering.Y > max_speed)
		velocity.Y = max_speed;
	else if (velocity.Y + steering.Y < -max_speed)
		velocity.Y = -max_speed;
	else
	{
		velocity.X += steering.X;
		velocity.Y += steering.Y;
	}
	
	newLocation.X = currentLocation.X + velocity.X * delta;
	newLocation.Y = currentLocation.Y + velocity.Y * delta;
	newLocation.Z = currentLocation.Z + velocity.Z * delta;
	SetActorLocation(newLocation);
	
}
void AVehicle::Flee(float delta)
{
	FVector newLocation;
	FVector currentLocation = this->GetActorLocation();
	FVector temp = player->GetPawn()->GetActorLocation() - currentLocation;

	if (velocity.X + acceleration < max_speed)
	{
		velocity.X = velocity.X + acceleration;
		velocity.Y = velocity.Y + acceleration;
	}

	FVector desired_velocity = temp * max_speed;
	FVector steering = -desired_velocity - velocity;

	if (steering.X > max_force)steering.X = max_force;
	if (steering.Y > max_force)steering.Y = max_force;
	steering /= mass;

	if (velocity.X + steering.X > max_speed)
		velocity.X = max_speed;
	else if (velocity.X + steering.X < -max_speed)
		velocity.X = -max_speed;
	if (velocity.Y + steering.Y > max_speed)
		velocity.Y = max_speed;
	else if (velocity.Y + steering.Y < -max_speed)
		velocity.Y = -max_speed;
	else
	{
		velocity.X += steering.X;
		velocity.Y += steering.Y;
	}

	newLocation.X = currentLocation.X + velocity.X * delta;
	newLocation.Y = currentLocation.Y + velocity.Y * delta;
	newLocation.Z = currentLocation.Z + velocity.Z * delta;
	SetActorLocation(newLocation);
}
void AVehicle::Pursue(float delta)
{
	FQuat rot;
	FVector temp = this->target - this->GetActorLocation();
	FVector desired_velocity = (FVector)temp.Normalize() * max_speed;
	FVector steering = desired_velocity - velocity;
	SetActorLocationAndRotation(steering, rot, false, 0, ETeleportType::None);
}

void AVehicle::Evade(float delta)
{
	FQuat rot;
	FVector temp = this->target - this->GetActorLocation();
	FVector desired_velocity = (FVector)temp.Normalize() * max_speed;
	FVector steering = desired_velocity - velocity;
	SetActorLocationAndRotation(steering, rot, false, 0, ETeleportType::None);
}

void AVehicle::Arrival(float delta)
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
	//APlayerController* FirstLocalPlayer = UGameplayStatics::GetPlayerController(this, 0);
	//InputComponent->BindAction(FName("MyAction"), IE_Pressed, this, &AVehicle::OnActionPressed);
	BindToInput();
	this->target = FVector(100.0, 100.0, 100.0);
	GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	player = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	UE_LOG(LogTemp, Warning, TEXT("Init Vehicle"));
}
void AVehicle::BindToInput()
{
	// Initialize our component
	InputComponent = NewObject<UInputComponent>(this);
	InputComponent->RegisterComponent();
	if (InputComponent)
	{
		// Bind inputs here
		InputComponent->BindAction("Jump", IE_Pressed, this, &AVehicle::OnActionPressed);
		// etc...
		UE_LOG(LogTemp, Warning, TEXT("Input Vehicle"));
		// Now hook up our InputComponent to one in a Player
		// Controller, so that input flows down to us
		EnableInput(GetWorld()->GetFirstPlayerController());
	}
}
void AVehicle::OnActionPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Key Pressed"));
}
// Called every frame
void AVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//FVector target;

	switch (GI->value)
	{
	case 0:
		Seek(DeltaTime);
		break;
	case 1:
		Flee(DeltaTime);
		break;
	case 2:
		Pursue(DeltaTime);
		break;
	case 3:
		Move(DeltaTime);
		break;
	}

}

