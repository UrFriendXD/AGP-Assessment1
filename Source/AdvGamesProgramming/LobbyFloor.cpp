// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyFloor.h"
#include "Engine/World.h"
#include "MultiplayerGameMode.h"


// Sets default values
ALobbyFloor::ALobbyFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALobbyFloor::BeginPlay()
{
	Super::BeginPlay();
	AssignLobbyFloor();
}

// Called every frame
void ALobbyFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALobbyFloor::AssignLobbyFloor()
{
	GameMode = Cast<AMultiplayerGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("Assigning lobby floor: GameMode cast worked"))
		GameMode->LobbyFloor = this;
	}

}

