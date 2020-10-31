// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralGeneration.h"


#include "EngineUtils.h"
#include "Room.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AProceduralGeneration::AProceduralGeneration()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    bReplicates = true;

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

    if (GetLocalRole() == ROLE_Authority)
    {
        Direction = FMath::RandRange(1, 5);
        ChooseStartingPoint();

        // Get procedural generation and add this to the RoomSpawners Array
        for (TActorIterator<ARoomSpawner> It(GetWorld()); It; ++It)
        {
            It->ProceduralGeneration = this;
            RoomSpawners.Add(*It);
        }

        // Find the procedural spawner in the level and assign variable
        for (TActorIterator<AProceduralSpawner> It(GetWorld()); It; ++It)
        {
            ProceduralSpawner = *It;
        }
    }
}

// Called every frame
void AProceduralGeneration::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (GetLocalRole() == ROLE_Authority)
    {
        // Choose new starting point if enabled. For debug purposes
        if (bChooseNewStartingPoint)
        {
            ChooseStartingPoint();
        }

        // If in bounds generate map
        if (!bIsOutOfBounds)
        {
            if (TimeBetweenRoom <= 0)
            {
                // Removes the room spawner it's on top of
                GetRoomSpawnerInPos();

                Move();

                TimeBetweenRoom = StartTimeBetweenRoom;
            }
            else
            {
                TimeBetweenRoom -= DeltaTime;
            }
        }
    }

    // if (bIsOutOfBounds && !bIsEverythingSpawned)
    // {
    //     // Populate nodes for AI to access
    //     PopulateAIManager();
    //
    //     //GetWorld()->SpawnActor<AAIManager>(GetActorLocation(), FRotator::ZeroRotator);
    //     
    //     if (GetLocalRole() == ROLE_Authority)
    //     {
    //         // Call generate AI and Pickups
    //         ProceduralSpawner->SpawnObjects();
    //     }
    //
    //     UE_LOG(LogTemp, Warning, TEXT("Manager"))
    //     if (GetLocalRole() == ROLE_AutonomousProxy)
    //     {
    //         UE_LOG(LogTemp, Warning, TEXT("Autonomous"))
    //     }
    //
    //     bIsEverythingSpawned = true;
    // }
}

// Removes room spawner from list
void AProceduralGeneration::RemoveRoomSpawner(ARoomSpawner* RoomSpawner)
{
    if (RoomSpawners.Contains(RoomSpawner))
    {
        RoomSpawners.Remove(RoomSpawner);
    }
}

void AProceduralGeneration::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AProceduralGeneration, bIsOutOfBounds);
    DOREPLIFETIME(AProceduralGeneration, bIsEverythingSpawned);
    DOREPLIFETIME(AProceduralGeneration, AIManager);
    DOREPLIFETIME(AProceduralGeneration, ProceduralSpawner);
}

// Choose a random starting point for the generation
void AProceduralGeneration::ChooseStartingPoint()
{
    int RandStartingPos = FMath::RandRange(0, StartingLocations.Num() - 1);
    SetActorLocation(StartingLocations[RandStartingPos]->GetActorLocation());
    int RandTemplateIndex = FMath::RandRange(0, RoomTemplates.Num() - 1);
    int RandIndex = FMath::RandRange(0, RoomTemplates[RandTemplateIndex].GetDefaultObject()->Rooms.Num() - 1);
    AActor* Room = GetWorld()->SpawnActor<AActor>(
        RoomTemplates[RandTemplateIndex].GetDefaultObject()->Rooms[RandIndex], GetActorLocation(),
        FRotator(0.f, 0.f, 0.f));
    bChooseNewStartingPoint = false;
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

            // Move the procedural generation object by move amount in the Right direction
            FVector NewPos = FVector(GetActorLocation().X, GetActorLocation().Y - MoveAmount, ZPos);
            SetActorLocation(NewPos);

            // Spawns a random room of a room shape variant
            int RandTemplateIndex = FMath::RandRange(0, RoomTemplates.Num() - 1);
            int RandIndex = FMath::RandRange(0, RoomTemplates[RandTemplateIndex].GetDefaultObject()->Rooms.Num() - 1);
            GetWorld()->SpawnActor<ARoom>(RoomTemplates[RandTemplateIndex].GetDefaultObject()->Rooms[RandIndex],
                                          GetActorLocation(), FRotator::ZeroRotator);

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

            // Move the procedural generation object by move amount in the Left direction
            FVector NewPos = FVector(GetActorLocation().X, GetActorLocation().Y + MoveAmount, ZPos);
            SetActorLocation(NewPos);

            // Spawns a random room of a room shape variant
            int RandTemplateIndex = FMath::RandRange(0, RoomTemplates.Num() - 1);
            int RandIndex = FMath::RandRange(0, RoomTemplates[RandTemplateIndex].GetDefaultObject()->Rooms.Num() - 1);
            GetWorld()->SpawnActor<ARoom>(RoomTemplates[RandTemplateIndex].GetDefaultObject()->Rooms[RandIndex],
                                          GetActorLocation(), FRotator::ZeroRotator);

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

            // Move the procedural generation object by move amount in the Down direction
            FVector NewPos = FVector(GetActorLocation().X + MoveAmount, GetActorLocation().Y, ZPos);
            SetActorLocation(NewPos);

            // Spawns a random that has a top opening
            int Rand = FMath::RandRange(2, 3);
            int RandIndex = FMath::RandRange(0, RoomTemplates[Rand].GetDefaultObject()->Rooms.Num() - 1);
            GetWorld()->SpawnActor<ARoom>(RoomTemplates[Rand].GetDefaultObject()->Rooms[RandIndex], GetActorLocation(),
                                          FRotator::ZeroRotator);

            // Choose any new direction
            Direction = FMath::RandRange(1, 5);
        }
        else
        {
            // Stop generating
            bIsOutOfBounds = true;

            // Spawns rooms in empty spaces
            SpawnEmptyRoom();
            
            // Populate nodes for AI to access
            PopulateAIManager();
            
            ProceduralSpawner->AIManager = AIManager;
            // Call generate AI and Pickups
            ProceduralSpawner->SpawnObjects();
        }
    }
}

void AProceduralGeneration::SpawnRoomWithBottom()
{
    // If it goes down twice, force spawn a room with UP and DOWN doors
    if (DownCounter >= 2)
    {
        int RandIndex = FMath::RandRange(0, RoomTemplates[3].GetDefaultObject()->Rooms.Num() - 1);
        GetWorld()->SpawnActor<ARoom>(RoomTemplates[3].GetDefaultObject()->Rooms[RandIndex], GetActorLocation(),
                                      FRotator::ZeroRotator);
    }
    else
    {
        // Else spawn a room with DOWN door
        int RandBottomRoom = FMath::RandRange(1, 3);
        if (RandBottomRoom == 2)
        {
            RandBottomRoom = 1;
        }
        int RandIndex = FMath::RandRange(0, RoomTemplates[RandBottomRoom].GetDefaultObject()->Rooms.Num() - 1);
        GetWorld()->SpawnActor<ARoom>(RoomTemplates[RandBottomRoom].GetDefaultObject()->Rooms[RandIndex],
                                      GetActorLocation(), FRotator::ZeroRotator);
    }
}

// Called when generation is complete to fill empty space with rooms. Should have all room spawners that it didn't touch along the path generation
void AProceduralGeneration::SpawnEmptyRoom()
{
    for (ARoomSpawner* RoomSpawner : RoomSpawners)
    {
        RoomSpawner->SpawnRoom();
    }
}

void AProceduralGeneration::PopulateAIManager()
{
    for (TActorIterator<AAIManager> It(GetWorld()); It; ++It)
    {
        AIManager = *It;
        if (AIManager)
        {
            AIManager->PopulateNodes();
            AIManager->PopulateCovers();
        }
    }
}
