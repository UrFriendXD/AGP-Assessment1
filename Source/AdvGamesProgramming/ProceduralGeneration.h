// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

    UPROPERTY(EditAnywhere)
    float MoveAmount;

    UPROPERTY(EditAnywhere)
    float ZPos;

    UPROPERTY(EditAnywhere)
    float StartTimeBetweenRoom;

    // Map size variables. Min Y is negative and going right, Max Y is positive and going left, Max X is going down with down positive
    UPROPERTY(EditAnywhere)
    float MinY;
    UPROPERTY(EditAnywhere)
    float MaxY;
    UPROPERTY(EditAnywhere)
    float MaxX;
    
private:
    void ChooseStartingPoint();

    int Direction;

    void Move();

    float TimeBetweenRoom;

    UPROPERTY(VisibleAnywhere)
    bool bIsOutOfBounds;
};
