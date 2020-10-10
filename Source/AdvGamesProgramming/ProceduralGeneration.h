// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "RoomSpawner.h"
#include "GameFramework/Actor.h"
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

    UPROPERTY(EditAnywhere, Category = "Rooms")
    TArray<TSubclassOf<AActor>> Rooms; // index 0 --> LR, index 1 --> LRB, index 2 --> LRT, index 3 --> LRBT

    // Move room amount
    UPROPERTY(EditAnywhere)
    float MoveAmount;

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

    UPROPERTY(EditAnywhere)
    TArray<TEnumAsByte<EObjectTypeQuery>> ActorsToCheck;
    UClass* ActorClass;

    UFUNCTION(BlueprintImplementableEvent)
    void CheckRoom();

    TArray<ARoomSpawner*> RoomSpawners;

    UFUNCTION(BlueprintImplementableEvent)
    void GetRoomSpawnerInPos();

    UFUNCTION(BlueprintCallable)
    void RemoveRoomSpawner(ARoomSpawner* RoomSpawner);

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
