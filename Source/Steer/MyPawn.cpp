// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "MyGameInstance.h"
#include "StateVehicle.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;
	//GetGameInstance();
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG
	(
		LogTemp,
		Display,
		TEXT("Init character")
	)
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector newLocation;
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!MovementDirection.IsZero() && MovableState(GI->value))
	{
		newLocation = GetActorLocation() + (MovementDirection * DeltaTime * 500.0);
		SetActorLocation(newLocation);
	}
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAction("Sprint", IE_Pressed, this, &AMyPawn::Sprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &AMyPawn::Walk);
	InputComponent->BindAction("Mode", IE_Pressed, this, &AMyPawn::ChangeModeVehicle);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyPawn::MoveRight);
	PlayerInputComponent->BindAxis("RotateRight", this, &AMyPawn::RotateRight);
}

void AMyPawn::ChangeModeVehicle()
{
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GI->ChangeModeVehicles();
	
}
void AMyPawn::Sprint()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Sprint"));
}
void AMyPawn::Walk()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Walk"));
}
void AMyPawn::MoveForward(float Value)
{		
	if(Value)
		velocity.X += Value;
	else velocity.X = 0;
	MovementDirection.X = FMath::Clamp(Value, -1.0, 1.0);
}
void AMyPawn::MoveRight(float Value)
{
	if (Value)
		velocity.Y += Value;
	else velocity.Y = 0;
	MovementDirection.Y = FMath::Clamp(Value, -1.0, 1.0);
}

void AMyPawn::RotateRight(float Value)
{
	AddActorLocalRotation(FRotator(0.0, Value, 0.0));
}
