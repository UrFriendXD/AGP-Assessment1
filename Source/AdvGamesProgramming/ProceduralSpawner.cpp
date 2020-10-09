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
		// Randomise number of AI, covers, pickups to spawn
		int NumCovers = FMath::FRandRange(2, 3);
		int NumAI = FMath::FRandRange(0, 2);
		int NumPickup = FMath::FRandRange(0, 1);
		int NumTotalSpawns = NumAI + NumCovers + NumPickup;

		// Add all NavNodes to an array, access the NavNodes thru the Room BP
		for (TActorIterator<ANavigationNode> NavNodeItr(GetWorld()); NavNodeItr; ++NavNodeItr)
		{
			AllNodesInRoom.Add(*NavNodeItr);
		}

		// For each cover to spawn, pick a random node and if it's NOT a door node, spawn covers
		for (int i = 0; i < NumTotalSpawns; i++)
		{
			int RandomIndex = FMath::FRandRange(0, AllNodesInRoom.Num() - 1);
			//while (AllNodesInRoom[RandomIndex]->IsA(ADoorNode::StaticClass) 
			//	|| AllNodesInRoom[RandomIndex]->bSpawnedSomething == true)
			//{
			//	RandomIndex = FMath::FRandRange(0, AllNodesInRoom.Num() - 1);
			//}

			if (NumCovers != 0)
			{
				Cover = nullptr;
				Cover = GetWorld()->SpawnActor<ACover>(CoverBlueprint, AllNodesInRoom[RandomIndex]->GetActorLocation(), FRotator::ZeroRotator);
				if (Cover)
				{
					// Destroy NavNodes under Cover
					AllNodesInRoom[RandomIndex]->Destroy();
					// Connect cover nodes to NavNodes via CoverBP
					Cover->CheckForNavNodes();

					//AllNodesInRoom[RandomIndex]->bSpawnedSomething = true;
					NumCovers--;
				}
			}
			else if (NumAI != 0)
			{
				// Spawn AI
				EnemyAI = nullptr;
				EnemyAI = GetWorld()->SpawnActor<AEnemyCharacter>(AEnemyCharacter::StaticClass(), AllNodesInRoom[RandomIndex]->GetActorLocation(), FRotator::ZeroRotator);
				if (EnemyAI)
				{
					NumAI--;
				}
			}
			else if (NumPickup != 0)
			{
				Pickup = nullptr;
				Pickup = GetWorld()->SpawnActor<APickup>(APickup::StaticClass(), AllNodesInRoom[RandomIndex]->GetActorLocation(), FRotator::ZeroRotator);
				if (Pickup)
				{
					NumPickup--;
				}
			}
		}
	}

}

