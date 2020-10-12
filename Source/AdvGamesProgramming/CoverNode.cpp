// Fill out your copyright notice in the Description page of Project Settings.


#include "CoverNode.h"

#include "DrawDebugHelpers.h"

void ACoverNode::BeginPlay()
{
	Super::BeginPlay();
	/*
	Super::CheckForNearbyNavNodes();
	for (auto It = ConnectedNodes.CreateConstIterator(); It; ++It)
	{
		DrawDebugLine(GetWorld(), GetActorLocation(), (*It)->GetActorLocation(), FColor::Blue, true);
	}
	*/
}
