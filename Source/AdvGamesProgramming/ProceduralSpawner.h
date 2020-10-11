// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Room.h"
#include "NavigationNode.h"
#include "Cover.h"
#include "CoverNode.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "EnemyCharacter.h"
#include "Pickup.h"
#include "DoorConnectionNode.h"
#include "ProceduralSpawner.generated.h"

UCLASS()
class ADVGAMESPROGRAMMING_API AProceduralSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralSpawner();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ANavigationNode*> AllNodesInRoom;
	UPROPERTY(EditAnywhere)
	AAIManager* AIManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ACover> CoverBlueprint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AEnemyCharacter> EnemyAIBlueprint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class APickup> PickupBlueprint;

	ACover* Cover;
	AEnemyCharacter* EnemyAI;
	APickup* Pickup;

	ARoom* CurrentRoom;

	//UFUNCTION(BlueprintImplementableEvent)
	//void CheckCoverNodeCollisions(FVector NodeLoc);

	void SpawnObjects();
};
