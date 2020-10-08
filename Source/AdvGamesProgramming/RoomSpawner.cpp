// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomSpawner.h"

#include "EngineUtils.h"
#include "ProceduralGeneration.h"

// Sets default values
ARoomSpawner::ARoomSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARoomSpawner::BeginPlay()
{
	Super::BeginPlay();
	for (TActorIterator<AProceduralGeneration> It(GetWorld()); It; ++It)
	{
		ProceduralGeneration = *It;
		ProceduralGeneration->RoomSpawners.Add(this);
	}
}

// Called every frame
void ARoomSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARoomSpawner::SpawnRoom()
{
	if (!bIsOnRoom)
	{
		int Random = FMath::RandRange(0, ProceduralGeneration->Rooms.Num()-1);
		GetWorld()->SpawnActor<AActor>(ProceduralGeneration->Rooms[Random], GetActorLocation(), FRotator::ZeroRotator);
	}
}

