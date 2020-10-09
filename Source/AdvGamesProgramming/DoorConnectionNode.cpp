// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorConnectionNode.h"

#include "DrawDebugHelpers.h"


void ADoorConnectionNode::BeginPlay()
{
    Super::BeginPlay();
    ADoorConnectionNode* DoorNode = CheckForNearbyDoorNode();
    if (!ConnectedNodes.Contains(DoorNode))
    {
        ConnectedNodes.Add(DoorNode);
        DoorNode->ConnectedNodes.Add(this);
    }   

    for (auto It = ConnectedNodes.CreateConstIterator(); It; ++It)
    {
        DrawDebugLine(GetWorld(), GetActorLocation(), (*It)->GetActorLocation(), FColor::Blue, true);
    }
}
