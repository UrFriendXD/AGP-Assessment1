// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "RoomSpawner.h"
#include "GameFramework/Actor.h"
#include "ProceduralSpawner.h"
#include "AIManager.h"
#include "RoomTemplates.h"

#include "ProceduralGeneration.generated.h"

UCLASS()
class ADVGAMESPROGRAMMING_API AProceduralGeneration : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AProceduralGeneration();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    USceneComponent* LocationComponent;

    UPROPERTY(EditAnywhere, Category="Starting Locations")
    TArray<AActor*> StartingLocations;

    UPROPERTY(EditAnywhere)
    bool bChooseNewStartingPoint;

    UPROPERTY(EditAnywhere, Category = "RoomTemplates")
    TArray<TSubclassOf<ARoomTemplates>> RoomTemplates; // index 0 --> LR, index 1 --> LRB, index 2 --> LRT, index 3 --> LRBT

    // Move room amount
    UPROPERTY(EditAnywhere)
    float MoveAmount;

    // Z pos of where the rooms should spawn at Default 150f
    UPROPERTY(EditAnywhere)
    float ZPos;

    // Timer
    UPROPERTY(EditAnywhere)
    float StartTimeBetweenRoom;

    // Map size variables. Min Y is negative and going right, Max Y is positive and going left, Max X is going down with down positive
    UPROPERTY(EditAnywhere)
    float MinY;
    UPROPERTY(EditAnywhere)
    float MaxY;
    UPROPERTY(EditAnywhere)
    float MaxX;

    // Calls to blueprint to cast overlap sphere and check the room type
    UFUNCTION(BlueprintImplementableEvent)
    void CheckRoom();

    // Array of all room spawners in the world
    TArray<ARoomSpawner*> RoomSpawners;

    // Uses BP to get the roomspawner it's ontop of
    UFUNCTION(BlueprintImplementableEvent)
    void GetRoomSpawnerInPos();

    // Removes the room spawner it's ontop of in the RoomSpawners array
    UFUNCTION(BlueprintCallable)
    void RemoveRoomSpawner(ARoomSpawner* RoomSpawner);

	UPROPERTY(EditAnywhere)
	AProceduralSpawner* ProceduralSpawner;

	UPROPERTY(EditAnywhere)
	AAIManager* AIManager;


private:
    void ChooseStartingPoint();

    UPROPERTY(VisibleAnywhere)
    int Direction;
    int DownCounter;

    void Move();

    float TimeBetweenRoom;

    UPROPERTY(VisibleAnywhere)
    bool bIsOutOfBounds;

    UFUNCTION(BlueprintCallable)
    void SpawnRoomWithBottom();

    void SpawnEmptyRoom();
};
