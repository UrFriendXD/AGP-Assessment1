// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EngineUtils.h"
#include "SelectionSet.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CurrentAgentState = AgentState::PATROL;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	PerceptionComponent = FindComponentByClass<UAIPerceptionComponent>();
	if (!PerceptionComponent) { UE_LOG(LogTemp, Error, TEXT("NO PERCEPTION COMPONENT FOUND")) }
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyCharacter::SensePlayer);
	
	HealthComponent = FindComponentByClass<UHealthComponent>();
	DetectedActor = nullptr;
	bBehindCover = false;
	bCanSeePlayer = false;
	HealTimer = HealDelay;
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CurrentAgentState)
	{
	case AgentState::PATROL:
		AgentPatrol();
		if (bHealingOthers)
		{
			CurrentAgentState = AgentState::HEALINGAGENTS;
			//Path.Empty();
		}
		if (bCanSeePlayer && HealthComponent->HealthPercentageRemaining() >= 0.4f)
		{
			CurrentAgentState = AgentState::ENGAGE;
			Path.Empty();
		}
		if (bCanSeePlayer && HealthComponent->HealthPercentageRemaining() <= 0.4f)
		{
			CurrentAgentState = AgentState::COVER;
			Path.Empty();
		}
		break;
	case AgentState::ENGAGE:
		AgentEngage();
		if (!bCanSeePlayer)
		{
			CurrentAgentState = AgentState::PATROL;
			FaceDirection = FRotator::ZeroRotator;
		}
		if (bCanSeePlayer && HealthComponent->HealthPercentageRemaining() <= 0.4f)
		{
			CurrentAgentState = AgentState::COVER;
			Path.Empty();
		}
		break;
	case AgentState::EVADE:
	{
		AgentCover();
		if (HealthComponent->HealthPercentageRemaining() == 1.0f)
		{
			bBehindCover = false;
			CurrentAgentState = AgentState::PATROL;
		}
		}
	}
			Path.Empty();
			bBehindCover = false;
			CurrentAgentState = AgentState::ENGAGE;
		else if (bCanSeeActor && HealthComponent->HealthPercentageRemaining() >= 0.4f)
		{
		AgentEvade();
		if (bHealingOthers)
		{
			CurrentAgentState = AgentState::HEALINGAGENTS;
			//Path.Empty();
		}
		if (!bCanSeePlayer)
		{
			CurrentAgentState = AgentState::PATROL;
		}
		if (bCanSeePlayer && HealthComponent->HealthPercentageRemaining() >= 0.4f)
		{
			CurrentAgentState = AgentState::ENGAGE;
			Path.Empty();
		}
		break;
		case AgentState::DEAD:
			if (HealthComponent->HealthPercentageRemaining() >= 1.0f)
			{
				CurrentAgentState = AgentState::PATROL;
			}
			break;
		case AgentState::HEALINGAGENTS:
			AgentHealing();
		break;
	default: ;
	}

	if (bEnemyHealing)
	{
		if (HealTimer <= 0)
		{
			Heal();
		}
	}

	if (HealTimer > 0)
	{
		HealTimer -= GetWorld()->GetDeltaSeconds();
	}
	MoveAlongPath();
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::AgentPatrol()
{
	if (Path.Num() == 0 && Manager != nullptr)
	{
		Path = Manager->GeneratePath(CurrentNode, Manager->AllNodes[FMath::RandRange(0, Manager->AllNodes.Num() - 1)]);
	}
}

void AEnemyCharacter::AgentEngage()
{
	if (bCanSeePlayer)
	{
		FVector DirectionToTarget = DetectedActor->GetActorLocation() - GetActorLocation();
		Fire(DirectionToTarget);
		FaceDirection = DirectionToTarget.Rotation();
		if (Path.Num() == 0)
		{
			Path = Manager->GeneratePath(CurrentNode, Manager->FindNearestNode(DetectedActor->GetActorLocation()));
		}
	}
	
}

void AEnemyCharacter::AgentEvade()
{
	if (bCanSeePlayer)
	{
		FVector DirectionToTarget = DetectedActor->GetActorLocation() - GetActorLocation();
		Fire(DirectionToTarget);
		if (Path.Num() == 0)
		{
			Path = Manager->GeneratePath(CurrentNode, Manager->FindFurthestNode(DetectedActor->GetActorLocation()));
		}
	}
}

void AEnemyCharacter::AgentCover()
{
	UE_LOG(LogTemp, Error, TEXT("Entered AgentCover state"));
	if (bCanSeeActor && Path.Num() == 0)
	{
		Path = Manager->GeneratePath(CurrentNode, Manager->FindFurthestCoverNode(DetectedActor->GetActorLocation()));
		UE_LOG(LogTemp, Error, TEXT("Generated new path to FurthestCoverNode"));
	}
	else if (bBehindCover && !bCanSeeActor)
	{
		UE_LOG(LogTemp, Error, TEXT("Behind cover and can't see player; healing."));
		//Heal();
	}

	if (Path.Num() == 0)
	{
		bBehindCover = true;
	}
	else
	{
		bBehindCover = false;
	}
}

void AEnemyCharacter::SensePlayer(AActor* SensedActor, FAIStimulus Stimulus)
void AEnemyCharacter::AgentHealing()
{
	if (bCanSeeEnemy)
	{
		const FVector DirectionToTarget = DetectedActor->GetActorLocation() - GetActorLocation();
		FaceDirection = DirectionToTarget.Rotation();
	}
	float Distance = FVector::Distance(DetectedActor->GetActorLocation(), GetActorLocation());
	UE_LOG(LogTemp, Display, TEXT("Distance is: %f"), Distance);
	if (Distance < 250.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Helping Friend"))
		Path.Empty();
		if (!Cast<AEnemyCharacter>(DetectedActor)->bEnemyHealing)
		{
			Cast<AEnemyCharacter>(DetectedActor)->bEnemyHealing = true;
		}
		if (Cast<AEnemyCharacter>(DetectedActor)->HealthComponent->HealthPercentageRemaining() >= 1.0f)
		{
			bHealingOthers = false;
			Cast<AEnemyCharacter>(DetectedActor)->bEnemyHealing = false;
			CurrentAgentState = AgentState::PATROL;
		}
	} else if (Path.Num() == 0)
	{
		Path = Manager->GeneratePath(CurrentNode, Manager->FindNearestNode(DetectedActor->GetActorLocation()));
	}
}

void AEnemyCharacter::SensePlayer(AActor* SensedActor, FAIStimulus Stimulus)
{
	if (CurrentAgentState != AgentState::DEAD)
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			if(SensedActor->ActorHasTag(TEXT("Player")) && !bHealingOthers)
			{
				DetectedActor = SensedActor;
				bCanSeePlayer = true;
				UE_LOG(LogTemp, Warning, TEXT("Player Detected"))
			}
			if (SensedActor->ActorHasTag(TEXT("Enemy")))
			{
				DetectedActor = SensedActor;
				bCanSeeEnemy = true;
				UE_LOG(LogTemp, Warning, TEXT("Enemy Detected"))
				if (Cast<AEnemyCharacter>(SensedActor)->HealthComponent->HealthPercentageRemaining() == 0 && !bHealingOthers)
				{
					bHealingOthers = true;
					CurrentAgentState = AgentState::HEALINGAGENTS;
					Path.Empty();
					Path = Manager->GeneratePath(CurrentNode, Manager->FindNearestNode(DetectedActor->GetActorLocation()));
					UE_LOG(LogTemp, Warning, TEXT("Enemy Needs help"))
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Player Lost"))
			bCanSeePlayer = false;
			if (!bHealingOthers)
			{
				bCanSeeEnemy = false;
			}
			//bHealing = false;
		}
	}
}

void AEnemyCharacter::MoveAlongPath()
{
	if (Path.Num() > 0 && Manager != NULL)
	{
		//UE_LOG(LogTemp, Display, TEXT("Current Node: %s"), *CurrentNode->GetName())
		if ((GetActorLocation() - CurrentNode->GetActorLocation()).IsNearlyZero(100.0f))
		{
			//UE_LOG(LogTemp, Display, TEXT("At Node %s"), *CurrentNode->GetName())
			CurrentNode = Path.Pop();
		}
		else
		{
			FVector WorldDirection = CurrentNode->GetActorLocation() - GetActorLocation();
			WorldDirection.Normalize();
			AddMovementInput(WorldDirection, 1.0f);

			//Get the AI to face in the direction of travel.
			if (CurrentAgentState != AgentState::ENGAGE && CurrentAgentState != AgentState::HEALINGAGENTS)
			{
				FaceDirection = WorldDirection.ToOrientationRotator();
			}
			FaceDirection.Roll = 0.f;
			FaceDirection.Pitch = 0.f;
			SetActorRotation(FaceDirection);
		}
	}
}

void AEnemyCharacter::Heal()
{
	if (HealthComponent->HealthPercentageRemaining() < 1)
	{
		HealthComponent->CurrentHealth += 3.0f;
		FMath::Clamp(HealthComponent->CurrentHealth, 0.0f, 100.0f);
		HealTimer = HealDelay;
		UE_LOG(LogTemp, Display, TEXT("Healed"));
	}
}
