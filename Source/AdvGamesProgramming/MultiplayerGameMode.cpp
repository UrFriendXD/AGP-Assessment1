// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameMode.h"
#include "EngineUtils.h"
#include "ProcedurallyGeneratedMap.h"
#include "PickupManager.h"
#include "Engine/GameEngine.h"
#include "GameFramework/HUD.h"
#include "TimerManager.h"
#include "PlayerHUD.h"
#include "Engine/World.h"
#include "GameFramework/GameState.h"
#include "PlayerCharacter.h"

void AMultiplayerGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessages)
{
	Super::InitGame(MapName, Options, ErrorMessages);
	HidingCountdownTime = 15;
	SeekingCountdownTime = 300;
	

	for (TActorIterator<AProceduralSpawner> It(GetWorld()); It; ++It)
	{
		ProceduralSpawnerClass = *It;
	}

	// //Find the procedurally generated map in the world
	// for (TActorIterator<AProcedurallyGeneratedMap> It(GetWorld()); It; ++It)
	// {
	// 	ProceduralMap = *It;
	// }

	//Spawn the pickup manager on the server
	// PickupManager = GetWorld()->SpawnActor<APickupManager>();
	//
	// //GetWorld()->SpawnActor<AAIManager>();
	//
	// //Initialise the pickup manager variables
	// if (PickupManager && ProceduralMap)
	// {
	// 	PickupManager->Init(ProceduralMap->Vertices, WeaponPickupClass, 10.0f);
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("Unable to find the procedural map or unable to spawn the pickup manager"));
	// }
}

void AMultiplayerGameMode::Respawn(AController* Controller)
{
	if (Controller)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("Respawning")));
		}

		//Hide the player hud as soon as the player dies.
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Controller->GetPawn()))
		{
			PlayerCharacter->HidePlayerHUD(true);
		}

		Controller->GetPawn()->SetLifeSpan(0.1f);

		//Set the respawn timer.

		FTimerHandle RespawnTimer;
		FTimerDelegate RespawnDelegate;
		RespawnDelegate.BindUFunction(this, TEXT("TriggerRespawn"), Controller);

		GetWorldTimerManager().SetTimer(RespawnTimer, RespawnDelegate, 5.0f, false);
	}
	//int test;
	
}

void AMultiplayerGameMode::TriggerRespawn(AController* Controller)
{
	if (Controller)
	{
		//Create the new player pawn
		AActor* SpawnPoint = ChoosePlayerStart(Controller);
		if (SpawnPoint)
		{
			//Spawn and take possession of the new player
			APawn* NewPlayerPawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, SpawnPoint->GetActorLocation(), SpawnPoint->GetActorRotation());
			if (NewPlayerPawn)
			{
				Controller->Possess(NewPlayerPawn);
				if (APlayerCharacter* Character = Cast<APlayerCharacter>(NewPlayerPawn))
				{
					UE_LOG(LogTemp, Display, TEXT("Showing the HUD"));
					Character->HidePlayerHUD(false);
				}
			}
		}
	}
}

/*
void AMultiplayerGameMode::StartMatch()
{
	
}
*/

void AMultiplayerGameMode::StartGame()
{
	if (LobbyFloor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found lobby floor!"));
		LobbyFloor->SetActorEnableCollision(false);
		
		// Set a random Seeker 
		int32 RandIndex = FMath::RandRange(0, GameState->PlayerArray.Num() - 1);
		int32 ItIndex = 0;
		for (TActorIterator<APlayerCharacter> It(GetWorld()); It; ++It)
		{
			if (ItIndex == RandIndex)
			{
				It->PlayerRole = PlayerRole::SEEKER;
				It->SetSeeker();
			}
			else
			{
				It->PlayerRole = PlayerRole::HIDER;
				It->SetHider();
			}
			ItIndex++;
		}

		GetWorld()->GetTimerManager().SetTimer(HidingTimerHandle, this, &AMultiplayerGameMode::HidingCountdown, 1.0f, true);
		
		ProceduralSpawnerClass->SpawnObjects();

		//Spawn pickups
		//Reset health and ammo
	}
}

void AMultiplayerGameMode::HidingCountdown()
{
	// Hiding countdown over
	if (--HidingCountdownTime <= 0)
	{
		GetWorld()->GetTimerManager().SetTimer(HidingTimerHandle, this, &AMultiplayerGameMode::SeekingCountdown, 1.0f, true);	
	}
	else // Hiding countdown ongoing
	{
		for (TActorIterator<APlayerCharacter> It(GetWorld()); It; ++It)
		{
			It->SetHidingTimerHUD(HidingCountdownTime);
		}
	}
}

void AMultiplayerGameMode::SeekingCountdown()
{
	if (--SeekingCountdownTime <= 0)
	{
		//Gameover: out of time
	}
	else // Seeking countdown ongoing
	{
		for (TActorIterator<APlayerCharacter> It(GetWorld()); It; ++It)
		{
			It->SetSeekingTimerHUD(SeekingCountdownTime);
		}
	}
}
