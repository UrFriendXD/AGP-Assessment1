// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "GameFramework/Actor.h"
#include "RoomSpawner.generated.h"

class AProceduralGeneration;
UCLASS()
class ADVGAMESPROGRAMMING_API ARoomSpawner : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ARoomSpawner();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    void SpawnRoom();

    UFUNCTION(BlueprintImplementableEvent)
    void CheckIfOnRoom();

    UPROPERTY(BlueprintReadWrite)
    bool bIsOnRoom;
private:
    AProceduralGeneration* ProceduralGeneration;
    
};
