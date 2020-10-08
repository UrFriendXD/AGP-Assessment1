// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralGeneration.h"

#include "Engine/World.h"

// Sets default values
AProceduralGeneration::AProceduralGeneration()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    LocationComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Location Component"));
    RootComponent = LocationComponent;
    StartTimeBetweenRoom = 0.25f;
    MoveAmount = 1000;
    ZPos = 150.0f;
}

// Called when the game starts or when spawned
void AProceduralGeneration::BeginPlay()
{
    Super::BeginPlay();

    Direction = FMath::RandRange(0, 5);
    ChooseStartingPoint();
}

// Called every frame
void AProceduralGeneration::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bChooseNewStartingPoint)
    {
        ChooseStartingPoint();
    }
    if (TimeBetweenRoom <= 0)
    {
        Move();
        TimeBetweenRoom = StartTimeBetweenRoom;
    }
    else
    {
        TimeBetweenRoom -= DeltaTime;
    }
}

void AProceduralGeneration::ChooseStartingPoint()
{
    int RandStartingPos = FMath::RandRange(0, StartingLocations.Num() - 1);
    SetActorLocation(StartingLocations[RandStartingPos]->GetActorLocation());
    AActor* Agent = GetWorld()->SpawnActor<AActor>(Rooms[0], GetActorLocation(), FRotator(0.f, 0.f, 0.f));
    bChooseNewStartingPoint = false;
}

void AProceduralGeneration::Move()
{
    if (Direction == 1 || Direction == 2)
    {
        FVector NewPos = FVector(GetActorLocation().X + MoveAmount, GetActorLocation().Y, ZPos);
        SetActorLocation(NewPos);
    }
    else if (Direction == 3 || Direction == 4)
    {
        FVector NewPos = FVector(GetActorLocation().X - MoveAmount, GetActorLocation().Y, ZPos);
        SetActorLocation(NewPos);
    }
    else if (Direction == 5)
    {
        FVector NewPos = FVector(GetActorLocation().X, GetActorLocation().Y - MoveAmount, ZPos);
        SetActorLocation(NewPos);
    }
    AActor* Agent = GetWorld()->SpawnActor<AActor>(Rooms[0], GetActorLocation(), FRotator::ZeroRotator);
    Direction = Direction = FMath::RandRange(0, 5);
}
