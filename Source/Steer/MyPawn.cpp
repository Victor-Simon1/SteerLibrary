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

	//Set Gravity
	/*FVector newLocation = this->GetActorLocation();
	newLocation.Z = newLocation.Z - 50.0 ;
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Mode %f"), newLocation.Z));
	SetActorLocation(newLocation);*/
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

void AMyPawn::RotateRight(float Value)
{
	if (GEngine)
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Je tourne"));
	AddActorLocalRotation(FRotator(0.0, Value, 0.0));
}
void AMyPawn::ChangeModeVehicle()
{
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	if (GI)
	{
		if (static_cast<int>(GI->value) < GI->maxValue - 1)
			GI->value = static_cast<StateVehicle>(static_cast<int>(GI->value)+1);
		else GI->value = StateVehicle::MOVE;
	}

	if (GEngine)
	{
		NameState(GI->value);
	}
		
	
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
	if (GEngine)
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Move %f"),Value));
	AddMovementInput(FVector::ForwardVector,Value);
	
}
void AMyPawn::MoveRight(float Value)
{
	if (GEngine)
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Move %f"), Value));
	AddMovementInput(FVector::RightVector, Value);

}