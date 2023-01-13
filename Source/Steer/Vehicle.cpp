// Fill out your copyright notice in the Description page of Project Settings.
#include "Vehicle.h"
#include "Math/Vector.h"
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
	state = StateVehicle::SEEK;
}

void AVehicle::Seek(const FVector &target)
{
	FVector temp = target - this->GetActorLocation();
	FVector desired_velocity = (FVector)temp.Normalize() * max_speed;
	FVector steering = desired_velocity - velocity;
}
void AVehicle::Flee(const FVector& target)
{
	FVector temp = target - this->GetActorLocation();
	FVector desired_velocity = (FVector)temp.Normalize() * max_speed;
	FVector steering = -desired_velocity - velocity;
}
void AVehicle::Pursue(const FVector& target)
{
	FVector temp = target - this->GetActorLocation();
	FVector desired_velocity = (FVector)temp.Normalize() * max_speed;
	FVector steering = desired_velocity - velocity;
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
	
}
void AVehicle::OnActionPressed()
{
	UE_LOG(LogTemp, Log, TEXT("Key Pressed"));
}
// Called every frame
void AVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector target;
	switch (state)
	{
	case StateVehicle::SEEK:
		Seek(target);
		break;
	case StateVehicle::FLEE:
		Flee(target);
		break;
	case StateVehicle::PURSUE:
		Pursue(target);
		break;
	}

}

