// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NavigationNode.h"
#include "Room.generated.h"

class ARoom;
class ANavigationNode;

UCLASS()
class ADVGAMESPROGRAMMING_API ARoom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int RoomType;

	UFUNCTION(BlueprintCallable)
	void RoomDestruction();

	UPROPERTY(EditAnywhere, Category = "Nav Nodes", BlueprintReadWrite)
	TArray<ANavigationNode*> ListOfNavNodes;
};
