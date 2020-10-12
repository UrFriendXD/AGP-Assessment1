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
	for (TActorIterator<AAIManager> It(GetWorld()); It; ++It)
	{
		AIManager = *It;
	}
	
}

// Called every frame
void AProceduralSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProceduralSpawner::SpawnObjects()
{
	// Randomise number of AI to spawn
	int NumAI = FMath::FRandRange(5, 8);
	UE_LOG(LogTemp, Warning, TEXT("NumAI to be spawned: %i"), NumAI)
	for (int i = 0; i < NumAI; i++)
	{
		int RandomIndex = FMath::FRandRange(0, AIManager->AllNodes.Num() - 1);
		// Spawn AI
		EnemyAI = nullptr;
		EnemyAI = GetWorld()->SpawnActor<AEnemyCharacter>(EnemyAIBlueprint, AIManager->AllNodes[RandomIndex]->GetActorLocation(), FRotator::ZeroRotator);
		if (EnemyAI)
		{
			EnemyAI->CurrentNode = AIManager->AllNodes[RandomIndex];
			EnemyAI->Path = AIManager->GeneratePath(EnemyAI->CurrentNode, AIManager->AllCoverNodes[FMath::RandRange(0, AIManager->AllCoverNodes.Num() - 1)]);
			UE_LOG(LogTemp, Warning, TEXT("AI spawned!"))
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AI not spawned"))
		}
	}

	// Iterate over all rooms to iterate over all navnodes in that room
	// to randomly spawn pickups (gun only, health pickups yet to be implemented)
	for (TActorIterator<ARoom> It(GetWorld()); It; ++It)
	{
		CurrentRoom = *It;
		AllNodesInRoom = CurrentRoom->ListOfNavNodes;

		// Randomise number of pickups to spawn
		int NumPickup = FMath::FRandRange(0, 2);
		for (int i = 0; i < NumPickup; i++)
		{
			int RandomIndex = FMath::FRandRange(0, AllNodesInRoom.Num() - 1);
			// Spawn pickups
			Pickup = nullptr;
			Pickup = GetWorld()->SpawnActor<APickup>(PickupBlueprint, AllNodesInRoom[RandomIndex]->GetActorLocation(), FRotator::ZeroRotator);
			if (Pickup)
			{
				UE_LOG(LogTemp, Warning, TEXT("Pickup spawned!"))
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Pickup not spawned"))
			}
		}
	}
}

