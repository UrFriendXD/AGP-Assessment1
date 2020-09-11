// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoverNode.h"
#include "Cover.h"
#include "NavigationNode.h"
#include "AIManager.generated.h"

class AEnemyCharacter;

UCLASS()
class ADVGAMESPROGRAMMING_API AAIManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAIManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category="AI Properties")
	int32 NumAI;
	UPROPERTY(VisibleAnywhere, Category = "Navigation Nodes")
	TArray<ANavigationNode*> AllNodes;

	UPROPERTY(VisibleAnywhere, Category = "Covers")
	TArray<ACover*> AllCovers;
	UPROPERTY(VisibleAnywhere, Category = "Cover Nodes")
	TArray<ACoverNode*> AllCoverNodes;

	UPROPERTY(VisibleAnywhere, Category = "Agents")
	TArray<AEnemyCharacter*> AllAgents;
	UPROPERTY(EditAnywhere, Category = "Agents")
	TSubclassOf<AEnemyCharacter> AgentToSpawn;

	TArray<ANavigationNode*> GeneratePath(ANavigationNode* StartNode, ANavigationNode* EndNode);

	/**
	Finds the nearest navigation node from the given location.
	@param Location - The location that you want to find the nearest node from.
	@return NearestNode - The nearest node to the given location.
	*/
	ANavigationNode* FindNearestNode(const FVector& Location);
	/**
	Finds the furthest navigation node from the given location.
	@param Location - The location that you want to find the furthest node from.
	@return FurthestNode - The furthest node from the given location.
	*/
	ANavigationNode* FindFurthestNode(const FVector& Location);

	// Finds the furthest cover node from the given location
	ACoverNode* FindFurthestCoverNode(const FVector& Location);

	void PopulateNodes();
	void CreateAgents();
	void PopulateCovers();
};
