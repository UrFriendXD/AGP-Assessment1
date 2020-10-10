// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralGeneration.h"


#include "EngineUtils.h"
#include "Room.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

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

    Direction = FMath::RandRange(1, 5);
    ChooseStartingPoint();

    // Get procedural generation and add this to the RoomSpawners Array
    for (TActorIterator<ARoomSpawner> It(GetWorld()); It; ++It)
    {
        It->ProceduralGeneration = this;
        RoomSpawners.Add(*It);
    }

	for (TActorIterator<AProceduralSpawner> It(GetWorld()); It; ++It)
	{
		ProceduralSpawner = *It;
	}
}

// Called every frame
void AProceduralGeneration::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bChooseNewStartingPoint)
    {
        ChooseStartingPoint();
    }

    if (!bIsOutOfBounds)
    {
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
}

void AProceduralGeneration::ChooseStartingPoint()
{
    int RandStartingPos = FMath::RandRange(0, StartingLocations.Num() - 1);
    SetActorLocation(StartingLocations[RandStartingPos]->GetActorLocation());
    AActor* Agent = GetWorld()->SpawnActor<AActor>(Rooms[0], GetActorLocation(), FRotator(0.f, 0.f, 0.f));
    bChooseNewStartingPoint = false;

    // Removes the room spawner it's on top of
    RoomSpawners.Remove(GetRoomSpawnerInPos());
}

void AProceduralGeneration::Move()
{
    // If direction is 1 or 2 go right
    if (Direction == 1 || Direction == 2)
    {
        // If within right bounds
        if (GetActorLocation().Y > MinY)
        {
            // Reset counter
            DownCounter = 0;

            // Get new pose and set actor to new pos
            FVector NewPos = FVector(GetActorLocation().X, GetActorLocation().Y - MoveAmount, ZPos);
            SetActorLocation(NewPos);
            //ARoomSpawner* RoomSpawner = GetRoomSpawnerInPos();
            //RoomSpawner->bRoomExists = true;
            // Removes the room spawner it's on top of
            RoomSpawners.Remove(GetRoomSpawnerInPos());

            // Spawns a random room
            int Rand = FMath::RandRange(0, Rooms.Num() - 1);
            GetWorld()->SpawnActor<ARoom>(Rooms[Rand], GetActorLocation(), FRotator::ZeroRotator);

            // Makes sure it can only move right or down
            Direction = FMath::RandRange(1, 5);
            if (Direction == 3)
            {
                Direction = 2;
            }
            else if (Direction == 4)
            {
                Direction = 5;
            }
        }
        else
        {
            // If reaches to the edge, move down
            Direction = 5;
        }
    }

        // Goes to the left
    else if (Direction == 3 || Direction == 4)
    {
        // Moves left if within bounds
        if (GetActorLocation().Y < MaxY)
        {
            // Reset counter
            DownCounter = 0;

            // Get new pose and set actor to new pos
            FVector NewPos = FVector(GetActorLocation().X, GetActorLocation().Y + MoveAmount, ZPos);
            SetActorLocation(NewPos);

            // Removes the room spawner it's on top of
            RoomSpawners.Remove(GetRoomSpawnerInPos());

            // Spawns a random room
            int Rand = FMath::RandRange(0, Rooms.Num() - 1);
            AActor* Agent = GetWorld()->SpawnActor<AActor>(Rooms[Rand], GetActorLocation(), FRotator::ZeroRotator);

            // Makes sure it can only move left or down
            Direction = FMath::RandRange(3, 5);
            if (Direction == 1)
            {
                Direction = 3;
            }
            else if (Direction == 2)
            {
                Direction = 5;
            }
        }
        else
        {
            // Else go down if at the edge
            Direction = 5;
        }
    }

        // Goes down
    else if (Direction == 5)
    {
        DownCounter++;

        // If not at the bottom
        if (GetActorLocation().X < MaxX)
        {
            // Check if current room has bottom opening if not make one
            CheckRoom();

            FVector NewPos = FVector(GetActorLocation().X + MoveAmount, GetActorLocation().Y, ZPos);
            SetActorLocation(NewPos);

            // Spawns a random that has a top opening
            int Rand = FMath::RandRange(2, 3);
            AActor* Agent = GetWorld()->SpawnActor<AActor>(Rooms[Rand], GetActorLocation(), FRotator::ZeroRotator);

            // Removes the room spawner it's on top of
            RoomSpawners.Remove(GetRoomSpawnerInPos());

            // Choose any new direction
            Direction = FMath::RandRange(1, 5);
        }
        else
        {
            // Stop generating
            bIsOutOfBounds = true;

            // Spawns rooms in empty spaces
            SpawnEmptyRoom();

            // Call generate Items and Stuffs
        }
    }
}

void AProceduralGeneration::SpawnRoomWithBottom()
{
    // If it goes down twice, force spawn a room with UP and DOWN doors
    if (DownCounter >= 2)
    {
        AActor* Agent = GetWorld()->SpawnActor<AActor>(Rooms[3], GetActorLocation(), FRotator::ZeroRotator);
    }
    else
    {
        // Else spawn a room with DOWN door
        int RandBottomRoom = FMath::RandRange(1, 3);
        if (RandBottomRoom == 2)
        {
            RandBottomRoom = 1;
        }
        AActor* Agent = GetWorld()->SpawnActor<AActor
        >(Rooms[RandBottomRoom], GetActorLocation(), FRotator::ZeroRotator);
    }

    // Removes the room spawner it's on top and if it is still in the list
    ARoomSpawner* RoomSpawner = GetRoomSpawnerInPos();
    if (RoomSpawners.Contains(RoomSpawner))
    {
        RoomSpawners.Remove(GetRoomSpawnerInPos());
    }
}

// Called when generation is complete to fill empty space with rooms
void AProceduralGeneration::SpawnEmptyRoom()
{
    for (ARoomSpawner* RoomSpawner : RoomSpawners)
    {
        RoomSpawner->SpawnRoom();
    }
}
