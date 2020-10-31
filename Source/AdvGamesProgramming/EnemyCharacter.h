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

    UPROPERTY(VisibleAnywhere)
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
    UPROPERTY(VisibleAnywhere)
    bool bCanSeeActor;

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
    UFUNCTION()
    void SensePlayer(AActor* ActorSensed, FAIStimulus Stimulus);
    UFUNCTION(BlueprintImplementableEvent)
    void Fire(FVector FireDirection);
    UFUNCTION(BlueprintImplementableEvent)
    void Reload();


    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // This property is used for healing animation
    UPROPERTY(BlueprintReadOnly)
    bool bIsHealingOthers;

private:

    void MoveAlongPath();
};
