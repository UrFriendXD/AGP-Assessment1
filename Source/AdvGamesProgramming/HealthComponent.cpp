// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Engine/GameEngine.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerHUD.h"
#include "PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "EnemyCharacter.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	MaxHealth = 100.0f;
	
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, CurrentHealth);
}

void UHealthComponent::OnTakeDamage(float Damage)
{
	if (CurrentHealth > 0)
	{
		CurrentHealth -= Damage;
	}
	if (CurrentHealth <= 0) 
	{
		CurrentHealth = 0;
		OnDeath();
	}

	if (GetOwner()->GetLocalRole() == ROLE_Authority)
	{
		UpdateHealthBar();
	}
}

void UHealthComponent::OnDeath()
{
	// Get enemy char script on agent 
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetOwner());
	APlayerCharacter* OwningPlayerCharacter = Cast<APlayerCharacter>(GetOwner());

	// If agent isn't already dead, set it to dead and clear current path
	if (EnemyCharacter->CurrentAgentState != AgentState::DEAD)
	{
		EnemyCharacter->CurrentAgentState = AgentState::DEAD;
		EnemyCharacter->Path.Empty();
	}
	else if (OwningPlayerCharacter)
	{
		OwningPlayerCharacter->OnDeath();
	}
}

float UHealthComponent::HealthPercentageRemaining()
{
	return CurrentHealth / MaxHealth;
}

void UHealthComponent::UpdateHealthBar()
{
	//If the owner of this health component is an autonomous proxy
	//NOTE: Possible to use function GetOwnerRole() as well! If you look at the 
	if (APlayerCharacter* OwningCharacter = Cast<APlayerCharacter>(GetOwner()))
	{
		if (OwningCharacter->IsLocallyControlled())
		{
			//Find the hud associated to this player
			APlayerHUD* HUD = Cast<APlayerHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
			if (HUD)
			{
				//Update the progress bar widget on the players hud.
				HUD->SetPlayerHealthBarPercent(HealthPercentageRemaining());
			}
		}
	}
}
