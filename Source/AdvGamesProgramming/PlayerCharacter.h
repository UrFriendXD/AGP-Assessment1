// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "MultiplayerGameState.h"
#include "PlayerCharacter.generated.h"

UENUM()
enum class PlayerRole : uint8
{
	SEEKER,
	HIDER
};

UCLASS()
class ADVGAMESPROGRAMMING_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float NormalMovementSpeed;
	float SprintMovementSpeed;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void MoveForward(float Value);
	void Strafe(float Value);
	void LookUp(float Value);
	void Turn(float Value);

	void SprintStart();
	void SprintEnd();

	void InteractStart();
	void InteractEnd();

	class UHealthComponent* HealthComponent;

	void OnDeath();

	/**
	 * Will adjust the movement speed of the server character to sprinting
	 */
	UFUNCTION(Server, Reliable)
	void ServerSprintStart();

	/**
	 * Will adjust the movement speed of the server character to normal walking speed
	 */
	UFUNCTION(Server, Reliable)
	void ServerSprintEnd();

	/**
	 * Client function that will hide or show the hud.
	 * @param bSetHudVisibility: Whether the hud should be hidden or shown.
	 */
	UFUNCTION(Client, Reliable)
	void HidePlayerHUD(bool bSetHUDVisibility);

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	PlayerRole PlayerRole;

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsInteracting;

	AMultiplayerGameState* GameState;
	APlayerState* HostState;

	bool bIsHost;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite)
	bool bIsPlayerHealing;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite)
	bool bIsDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<APlayerCharacter*> TrackPlayers;

	UFUNCTION(BlueprintImplementableEvent)
	void HealHiders();
	UFUNCTION(BlueprintImplementableEvent)
	void HealHiders_END();

private:
	UPROPERTY(EditInstanceOnly, meta = (ClampMin = "0.0", ClampMax = "2.0", UIMin = "0.0", UIMax = "2.0"))
	float LookSensitivity;

	UPROPERTY(EditInstanceOnly)
	float MoveSpeed;

	UPROPERTY(EditInstanceOnly)
	float SprintMultiplier;

	UCameraComponent* Camera;

	// Healing timer
	UPROPERTY(VisibleAnywhere)
	float HealTimer;
	UPROPERTY(EditAnywhere)
	float HealDelay;

	void Heal();

};
