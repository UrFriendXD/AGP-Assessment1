// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralSpawner.h"

// Sets default values
AProceduralSpawner::AProceduralSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AProceduralSpawner::BeginPlay()
{
	Super::BeginPlay();
	AllNodesInRoom.Empty();
	
}

// Called every frame
void AProceduralSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProceduralSpawner::SpawnObjects()
{
	// Iterate over all rooms to iterate over all navnodes 
	// to randomly spawn 0-2 AI, 2-3 covers, 0-1 pickup (gun/health)
	for (TActorIterator<ARoom> It(GetWorld()); It; ++It)
	{
		CurrentRoom = *It;
		AllNodesInRoom = CurrentRoom->ListOfNavNodes;

		// Randomise number of AI, covers, pickups to spawn
		int NumAI = FMath::FRandRange(0, 2);
		int NumPickup = FMath::FRandRange(0, 1);
		int NumTotalSpawns = NumAI + NumPickup;

		// For each cover to spawn, pick a random node and if it's NOT a door node, spawn covers
		for (int i = 0; i < NumTotalSpawns; i++)
		{
			
			int RandomIndex = FMath::FRandRange(0, AllNodesInRoom.Num() - 1);
			/*while (AllNodesInRoom[RandomIndex]->IsA(ADoorConnectionNode::StaticClass) 
				|| AllNodesInRoom[RandomIndex]->bSpawnedSomething == true)
			{
				RandomIndex = FMath::FRandRange(0, AllNodesInRoom.Num() - 1);
			}
			*/

			if (NumAI != 0)
			{
				// Spawn AI
				EnemyAI = nullptr;
				EnemyAI = GetWorld()->SpawnActor<AEnemyCharacter>(EnemyAIBlueprint, AllNodesInRoom[RandomIndex]->GetActorLocation(), FRotator::ZeroRotator);
				if (EnemyAI)
				{
					NumAI--;
					UE_LOG(LogTemp, Warning, TEXT("AI spawned!"))
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("AI not spawned"))
				}
			}
			else if (NumPickup != 0)
			{
				Pickup = nullptr;
				Pickup = GetWorld()->SpawnActor<APickup>(PickupBlueprint, AllNodesInRoom[RandomIndex]->GetActorLocation(), FRotator::ZeroRotator);
				if (Pickup)
				{
					NumPickup--;
					UE_LOG(LogTemp, Warning, TEXT("Pickup spawned!"))
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Pickup not spawned"))
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Did not get to spawn anything..."))
			}
		}
	}

}

