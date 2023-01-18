// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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

}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAction("Sprint", IE_Pressed, this, &AMyPawn::Sprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &AMyPawn::Walk);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyPawn::MoveRight);

}

void AMyPawn::ChangeModeVehicle()
{
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI)
	{
		// Do Something
		GI->value++;
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
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Move %f"),Value));
	AddMovementInput(FVector::BackwardVector,Value);
	
}
void AMyPawn::MoveRight(float Value)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Move %f"), Value));
	AddMovementInput(FVector::RightVector, Value);

}