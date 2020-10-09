// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorConnectionNode.h"

#include "DrawDebugHelpers.h"


void ADoorConnectionNode::BeginPlay()
{
    Super::BeginPlay();
    CheckForNearbyDoorNode();
    
    for (auto It = ConnectedNodes.CreateConstIterator(); It; ++It)
    {
        DrawDebugLine(GetWorld(), GetActorLocation(), (*It)->GetActorLocation(), FColor::Blue, true);
    }
}

void ADoorConnectionNode::AddConnectionNode(ADoorConnectionNode* DoorConnectionNode)
{
    if (!ConnectedNodes.Contains(DoorConnectionNode))
    {
        ConnectedNodes.Add(DoorConnectionNode);
        DoorConnectionNode->ConnectedNodes.Add(this);
    }
}
