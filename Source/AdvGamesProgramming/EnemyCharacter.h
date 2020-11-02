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
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(VisibleAnywhere)
    TArray<ANavigationNode*> Path;
    UPROPERTY(Replicated, VisibleAnywhere)
    ANavigationNode* CurrentNode;
    UPROPERTY(Replicated, VisibleAnywhere)
    AAIManager* Manager;

    UPROPERTY(VisibleAnywhere)
    UAIPerceptionComponent* PerceptionComponent;

    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite)
    AgentState CurrentAgentState;
    UPROPERTY(VisibleAnywhere)
    AActor* DetectedActor;
    UPROPERTY(VisibleAnywhere)
    bool bCanSeePlayer;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bCanHearPlayer;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bCanSeeEnemy;
    UPROPERTY(VisibleAnywhere)
    bool bBehindCover;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool bOutOfAmmo;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool bIsReloading;

    UHealthComponent* HealthComponent;

    UPROPERTY(EditAnywhere, meta = (UIMin = "10.0", UIMax = "1000.0", ClampMin = "10.0", ClampMax = "1000.0"))
    float PathfindingNodeAccuracy;

    UFUNCTION()
    void AgentPatrol();
    UFUNCTION()
    void AgentEngage();
    // UFUNCTION()
    // void AgentEvade();
    UFUNCTION(BlueprintCallable)
    void AgentHealing();
    UFUNCTION(BlueprintCallable)
    void AgentCover();
    UFUNCTION()
    void SensePlayer(AActor* ActorSensed, FAIStimulus Stimulus);
    
    UFUNCTION(BlueprintImplementableEvent)
    void Fire(FVector FireDirection);
    UFUNCTION(BlueprintImplementableEvent)
    void Reload();

    UFUNCTION()
    void Heal();

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
    bool bEnemyHealing;

    UPROPERTY(EditAnywhere)
    float HealDelay;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // This property is used for healing animation
    UPROPERTY(Replicated, BlueprintReadOnly)
    bool bIsHealingOthers;

private:

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
