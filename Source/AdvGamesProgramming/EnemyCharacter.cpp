// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EngineUtils.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    CurrentAgentState = AgentState::PATROL;
    ReviveDelay = 2.0f;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();

    PerceptionComponent = FindComponentByClass<UAIPerceptionComponent>();
    if (!PerceptionComponent) { UE_LOG(LogTemp, Error, TEXT("NO PERCEPTION COMPONENT FOUND")) }
    PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyCharacter::SensePlayer);

    // Initialise variables
    HealthComponent = FindComponentByClass<UHealthComponent>();
    DetectedActor = nullptr;
    bBehindCover = false;
    bCanSeePlayer = false;
    bCanHearPlayer = false;
    HealTimer = HealDelay;
    ReviveTimer = ReviveDelay;
    for (TActorIterator<AAIManager> It(GetWorld()); It; ++It)
    {
        Manager = *It;
        UE_LOG(LogTemp, Warning, TEXT("AIManager assigned"))
    }
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    switch (CurrentAgentState)
    {
        // Patrol normally
    case AgentState::PATROL:
        AgentPatrol();
        // Change to healing state if healing
        if (bHealingOthers)
        {
            CurrentAgentState = AgentState::HEALINGAGENTS;
            Path.Empty();
        }

        // Change to engage state if healthy
        if (bCanSeePlayer && HealthComponent->HealthPercentageRemaining() >= 0.4f)
        {
            CurrentAgentState = AgentState::ENGAGE;
            Path.Empty();
        }

        // Change to Cover state if low
        if (bCanSeePlayer && HealthComponent->HealthPercentageRemaining() <= 0.4f)
        {
            CurrentAgentState = AgentState::COVER;
            Path.Empty();
        }
        break;

        // Engage state
    case AgentState::ENGAGE:
        AgentEngage();

        // Change to patrol state if lost player
        if (!bCanSeePlayer)
        {
            CurrentAgentState = AgentState::PATROL;
            FaceDirection = FRotator::ZeroRotator;
        }

        // Change to Cover state if low
        if (bCanSeePlayer && HealthComponent->HealthPercentageRemaining() <= 0.4f)
        {
            CurrentAgentState = AgentState::COVER;
            Path.Empty();
        }
        break;

        // Cover state when HP is < 40%
    case AgentState::COVER:
        AgentCover();

        // Change to healing state if healing
        if (bHealingOthers)
        {
            CurrentAgentState = AgentState::HEALINGAGENTS;
            //Path.Empty();
        }

        // Change to patrol state if healthy
        if (!bCanSeePlayer && HealthComponent->HealthPercentageRemaining() >= 0.9f)
        {
            CurrentAgentState = AgentState::PATROL;
            bBehindCover = false;
        }

            // Change to Engage state if healthy and sees player
        else if (bCanSeePlayer && HealthComponent->HealthPercentageRemaining() >= 0.4f)
        {
            CurrentAgentState = AgentState::ENGAGE;
            Path.Empty();
            bBehindCover = false;
        }
        break;

        // Dead state
    case AgentState::DEAD:
        // Revive if full HP
        if (HealthComponent->HealthPercentageRemaining() >= 1.0f)
        {
            CurrentAgentState = AgentState::REVIVING;
        }
        break;

        // Healing other agents state
    case AgentState::HEALINGAGENTS:
        AgentHealing();
        break;

    case AgentState::REVIVING:
        if (ReviveTimer <= 0)
        {
            CurrentAgentState = AgentState::PATROL;
            ReviveTimer = ReviveDelay;
        }
        else
            ReviveTimer -= DeltaTime;

    default: ;
    }

    // If being healed, heal
    if (bEnemyHealing)
    {
        if (HealTimer <= 0)
        {
            Heal();
        }
    }

    // Timer
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

// Replaces Evade function
void AEnemyCharacter::AgentCover()
{
    // Generates a path to the furthest cover node from the Player
    if (bCanSeePlayer && Path.Num() == 0)
    {
        Path = Manager->GeneratePath(CurrentNode, Manager->FindFurthestCoverNode(DetectedActor->GetActorLocation()));
        //UE_LOG(LogTemp, Error, TEXT("Generated new path to FurthestCoverNode"));
    }
        // Only heals if it's behind cover and cannot see the Player
    else if (bBehindCover && !bCanSeePlayer)
    {
        // Make sure it doesn't heal too fast (not every tick)
        if (HealTimer <= 0)
        {
            Heal();
        }
    }

    // Ensuring that it's only marked as behind cover when it's not navigating
    if (Path.Num() == 0)
    {
        bBehindCover = true;
    }
    else
    {
        bBehindCover = false;
    }
}

void AEnemyCharacter::AgentHealing()
{
    // If can see enemy face enemy
    if (bCanSeeEnemy)
    {
        const FVector DirectionToTarget = DetectedActor->GetActorLocation() - GetActorLocation();
        FaceDirection = DirectionToTarget.Rotation();
    }
    float Distance = FVector::Distance(DetectedActor->GetActorLocation(), GetActorLocation());
    //UE_LOG(LogTemp, Warning, TEXT("Distance %f"), Distance);

    // If distance of enemy and dead enemy < X, heal the enemy
    if (Distance < 200.0f)
    {
        //UE_LOG(LogTemp, Warning, TEXT("Helping Friend"));
        Path.Empty();
        if (!Cast<AEnemyCharacter>(DetectedActor)->bEnemyHealing)
        {
            Cast<AEnemyCharacter>(DetectedActor)->bEnemyHealing = true;
        }

        // If they're full on HP, go back to patrol
        if (Cast<AEnemyCharacter>(DetectedActor)->HealthComponent->HealthPercentageRemaining() >= 1.0f)
        {
            bHealingOthers = false;
            Cast<AEnemyCharacter>(DetectedActor)->bEnemyHealing = false;
            CurrentAgentState = AgentState::PATROL;
        }
    }

        // If path is 0, go to enemy
    else if (Path.Num() == 0)
    {
        Path = Manager->GeneratePath(CurrentNode, Manager->FindNearestNode(DetectedActor->GetActorLocation()));
    }
}

void AEnemyCharacter::SensePlayer(AActor* SensedActor, FAIStimulus Stimulus)
{
    // If not dead sense
    if (CurrentAgentState != AgentState::DEAD)
    {
        if (Stimulus.WasSuccessfullySensed())
        {
            switch (Stimulus.Type)
            {
                // Sight sensing
            case 0:
                // If player is detected and this agent isn't healing, agent can see player, detectedActor = player
                if (SensedActor->ActorHasTag(TEXT("Player")) && !bHealingOthers)
                {
                    DetectedActor = SensedActor;
                    bCanSeePlayer = true;
                    UE_LOG(LogTemp, Warning, TEXT("Player Detected"))
                }

                // If enemy is detected and this agent isn't healing, agent can see an enemy, detectedActor = enemy
                if (SensedActor->ActorHasTag(TEXT("Enemy")))
                {
                    DetectedActor = SensedActor;
                    bCanSeeEnemy = true;
                    UE_LOG(LogTemp, Warning, TEXT("Enemy Detected"))

                    // If enemy is dead and not healing set healing others to true and enter healingAgents state
                    if (Cast<AEnemyCharacter>(SensedActor)->HealthComponent->HealthPercentageRemaining() == 0 && !
                        bHealingOthers && !Cast<AEnemyCharacter>(SensedActor)->bEnemyHealing)
                    {
                        bHealingOthers = true;
                        CurrentAgentState = AgentState::HEALINGAGENTS;
                        Path.Empty();
                        Path = Manager->GeneratePath(CurrentNode,
                                                     Manager->FindNearestNode(DetectedActor->GetActorLocation()));
                        UE_LOG(LogTemp, Warning, TEXT("Enemy Needs help"))
                    }
                }
                break;

                // Hearing sensing
            case 1:
                // If not healing and hearing target is a player
                if (SensedActor->ActorHasTag(TEXT("Player")) && !bHealingOthers)
                {
                    DetectedActor = SensedActor;
                    bCanHearPlayer = true;
                    //UE_LOG(LogTemp, Warning, TEXT("Player Heard"))
                }
                break;
            default:
                UE_LOG(LogTemp, Warning, TEXT("Unknown Stimulus detected"));
            }
        }

            // If sensing fails/lost set values to false
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Player Lost"))
            bCanSeePlayer = false;
            bCanHearPlayer = false;

            // If healing don't set canSeeEnemy to false
            if (!bHealingOthers)
            {
                bCanSeeEnemy = false;
            }
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

            //Get the AI to face in the direction of travel if not healing or engaging 
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

// Heals enemy agent
void AEnemyCharacter::Heal()
{
    // Gets AI to look at player if they can hear the player - and when they do, run to another cover
    if (bCanHearPlayer && CurrentAgentState != AgentState::DEAD)
    {
        const FVector DirectionToTarget = DetectedActor->GetActorLocation() - GetActorLocation();
        FaceDirection = DirectionToTarget.Rotation();
        FaceDirection.Roll = 0.0f;
        FaceDirection.Pitch = 0.0f;
        SetActorRotation(FaceDirection);
    }
        // Else if AI cannot sense the player through sight or sound, start healing
    else if (HealthComponent->HealthPercentageRemaining() < 1)
    {
        HealthComponent->CurrentHealth += 3.0f;
        FMath::Clamp(HealthComponent->CurrentHealth, 0.0f, 100.0f);
        HealTimer = HealDelay;
        UE_LOG(LogTemp, Display, TEXT("Healed"));
    }
}
