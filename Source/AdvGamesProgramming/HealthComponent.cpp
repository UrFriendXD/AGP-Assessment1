// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "EnemyCharacter.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	MaxHealth = 100.0f;
	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
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
}

void UHealthComponent::OnDeath()
{
	// Get enemy char script on agent 
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetOwner());

	// If agent isn't already dead, set it to dead and clear current path
	if (EnemyCharacter->CurrentAgentState != AgentState::DEAD)
	{
		EnemyCharacter->CurrentAgentState = AgentState::DEAD;
		EnemyCharacter->Path.Empty();
	}
}

float UHealthComponent::HealthPercentageRemaining()
{
	return CurrentHealth / MaxHealth;
}
