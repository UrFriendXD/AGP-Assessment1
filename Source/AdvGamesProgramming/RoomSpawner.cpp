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
}

// Called every frame
void ARoomSpawner::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ARoomSpawner::SpawnRoom()
{
    // Spawns a random room at it's location
    int RandTemplateIndex = FMath::RandRange(0, ProceduralGeneration->RoomTemplates.Num() - 1);
    int RandIndex = FMath::RandRange(0, ProceduralGeneration->RoomTemplates[RandTemplateIndex].GetDefaultObject()->Rooms.Num() - 1);
    GetWorld()->SpawnActor<ARoom>(ProceduralGeneration->RoomTemplates[RandTemplateIndex].GetDefaultObject()->Rooms[RandIndex],
                                  GetActorLocation(), FRotator::ZeroRotator);
}
