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
	
	// Iterate over all rooms to iterate over all navnodes 
	// to randomly spawn 0-2 AI, 2-3 covers, 0-1 pickup (gun/health)
	for (TActorIterator<ARoom> It(GetWorld()); It; ++It)
	{
		// Randomise number of AI, covers, pickups to spawn
		int NumAI = FMath::FRandRange(0, 2);
		int NumCovers = FMath::FRandRange(2, 3);
		int NumPickup = FMath::FRandRange(0, 1);
		int NumTotalSpawns = NumAI + NumCovers + NumPickup;

		
		// Add all NavNodes to an array
		for (TActorIterator<ANavigationNode> It(GetWorld()); It; ++It)
		{
			
		}

		// For each thing to spawn, pick a random node and if it's NOT a door node, spawn covers
		for (int i = 0; i < NumTotalSpawns; i++)
		{

		}

		
		
		// Do cover and cover node stuff
		// Pick a random node to spawn AI and pickup


		/*
		for (TActorIterator<ANavigationNode> It(GetWorld()); It; ++It)
		{
			// If the node is NOT a Door Node, spawn cover
			if (It->IsA(ACoverNode::StaticClass))
			{

			}
		}
		*/
	}
	
}

// Called every frame
void AProceduralSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

