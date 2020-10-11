// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Room.h"
#include "NavigationNode.generated.h"

class ARoom;
class ANavigationNode;

UCLASS()
class ADVGAMESPROGRAMMING_API ANavigationNode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANavigationNode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Connected Nodes", BlueprintReadOnly)
	TArray<ANavigationNode*> ConnectedNodes;

	USceneComponent* LocationComponent;

	float GScore;
	float HScore;
	float FScore();

	ANavigationNode* CameFrom;

	bool bSpawnedSomething;

	UFUNCTION(BlueprintImplementableEvent)
	void CheckForNearbyNavNodes();

	UFUNCTION(BlueprintCallable)
	void AddToConnectedNodes(ANavigationNode* NavigationNode);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ARoom* Room;

};
