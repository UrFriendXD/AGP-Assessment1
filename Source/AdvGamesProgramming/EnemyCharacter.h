// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavigationNode.h"
#include "GameFramework/Character.h"
#include "AIManager.h"
#include "Perception/AIPerceptionComponent.h"
#include "HealthComponent.h"
#include "EnemyCharacter.generated.h"

UENUM()
enum class AgentState : uint8
{
    PATROL,
    ENGAGE,
    HEALINGAGENTS,
    DEAD,
    COVER,
    REVIVING
};

UCLASS()
class ADVGAMESPROGRAMMING_API AEnemyCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    AEnemyCharacter();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TArray<ANavigationNode*> Path;
    UPROPERTY(VisibleAnywhere)
    ANavigationNode* CurrentNode;
    UPROPERTY(VisibleAnywhere)
    AAIManager* Manager;

    UPROPERTY(VisibleAnywhere)
    UAIPerceptionComponent* PerceptionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    AgentState CurrentAgentState;
    UPROPERTY(VisibleAnywhere)
    AActor* DetectedActor;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bCanSeePlayer;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bCanHearPlayer;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bCanSeeEnemy;
    UPROPERTY(VisibleAnywhere)
    bool bBehindCover;

    UHealthComponent* HealthComponent;

    // UFUNCTION(BlueprintCallable)
    // void AgentPatrol();
    UFUNCTION(BlueprintCallable)
    void AgentEngage();
    // UFUNCTION(BlueprintCallable)
    // void AgentEvade();
    UFUNCTION(BlueprintCallable)
    void AgentHealing();
    UFUNCTION(BlueprintCallable)
    void AgentCover();
    UFUNCTION(BlueprintCallable)
    void SensePlayer(AActor* ActorSensed, FAIStimulus Stimulus);
    UFUNCTION(BlueprintImplementableEvent)
    void Fire(FVector FireDirection);

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UPROPERTY(EditAnywhere, Category=Behavior)
    class UBehaviorTree* EnemyCharacterBehavior;

    UFUNCTION()
    void Heal();

    UPROPERTY(EditAnywhere)
    bool bEnemyHealing;

    UPROPERTY(EditAnywhere)
    float HealDelay;

    // This property is used for healing animation
    UPROPERTY(BlueprintReadOnly)
    bool bIsHealingOthers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite )
    bool bHasAmmo;

private:

    UFUNCTION(BlueprintCallable)
    void MoveAlongPath();

    UPROPERTY(EditAnywhere)
    bool bUnderCover;

    UPROPERTY(VisibleAnywhere)
    bool bHealingOthers;
    
    // Healing timer
    UPROPERTY(VisibleAnywhere)
    float HealTimer;
    
    bool bTransitioningIntoCover;

    FRotator FaceDirection;

    // Reviver timers
    UPROPERTY(VisibleAnywhere)
    float ReviveTimer;
    UPROPERTY(EditAnywhere)
    float ReviveDelay;

    // Finding new cover timers
    UPROPERTY(VisibleAnywhere)
    float FindNewCoverTimer;
    UPROPERTY(EditAnywhere)
    float FindNewCoverDelay;
};
