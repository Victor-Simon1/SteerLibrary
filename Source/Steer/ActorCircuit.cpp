// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorCircuit.h"

// Sets default values
AActorCircuit::AActorCircuit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Super Mesh"));
	mesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BaseMeshAsset(TEXT("StaticMesh'/Game/StarterContent/Props/MaterialSphere.MaterialSphere'"));

	if (BaseMeshAsset.Object)
	{
		mesh->SetStaticMesh(BaseMeshAsset.Object);
	}
}

// Called when the game starts or when spawned
void AActorCircuit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActorCircuit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

