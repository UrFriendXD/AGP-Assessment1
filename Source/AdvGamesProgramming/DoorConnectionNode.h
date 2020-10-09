// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavigationNode.h"
#include "DoorConnectionNode.generated.h"

/**
 * 
 */
UCLASS()
class ADVGAMESPROGRAMMING_API ADoorConnectionNode : public ANavigationNode
{
    GENERATED_BODY()

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    public:
    UFUNCTION(BlueprintImplementableEvent)
    ADoorConnectionNode* CheckForNearbyDoorNode();
};
