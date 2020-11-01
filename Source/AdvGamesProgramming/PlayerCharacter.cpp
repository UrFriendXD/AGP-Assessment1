// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "HealthComponent.h"
#include "MultiplayerGameMode.h"
#include "PlayerHUD.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//AutoPossessPlayer = EAutoReceiveInput::Player0;
	bUseControllerRotationPitch = true;

	LookSensitivity = 1.0f;
	SprintMultiplier = 1.5f;

	//Set the normal and sprint movement speeds
	NormalMovementSpeed = GetCharacterMovement()->MaxWalkSpeed;
	SprintMovementSpeed = GetCharacterMovement()->MaxWalkSpeed * SprintMultiplier;

	// Role
	PlayerRole = PlayerRole::HIDER;
	
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	Camera = FindComponentByClass<UCameraComponent>();
	HealthComponent = FindComponentByClass<UHealthComponent>();
	if (HealthComponent)
	{
		HealthComponent->SetIsReplicated(true);
	}

	GameState = Cast<AMultiplayerGameState>(GetWorld()->GetGameState());
	if (GetPlayerState() == GameState->PlayerArray[0])
	{
		bIsHost = true;
	}
	//HostState = GameState->PlayerArray[0];

	// If is host, set host HUD to 1/4 Players to begin with, no RoleText, hide TimerText - have a StartGame button in place
	if (IsLocallyControlled())
	{
		if (bIsHost)
		{
			if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
			{
				if (APlayerHUD* HUD = Cast<APlayerHUD>(PlayerController->GetHUD()))
				{
					HUD->SetNumPlayersText(1);
					HUD->SetRoleText(TEXT(""));
					HUD->SetHideTimerText(true);
					HUD->SetHideStartGameButton(false);
				}
			}
		}
		else
		{
			if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
			{
				if (APlayerHUD* HUD = Cast<APlayerHUD>(PlayerController->GetHUD()))
				{
					int32 NumPlayers = GameState->PlayerArray.Num();

					HUD->SetNumPlayersText(NumPlayers);
					HUD->SetRoleText(TEXT(""));
					HUD->SetHideTimerText(false);
					HUD->SetWaitingForHostTimerText();
					HUD->SetHideStartGameButton(true);
				}
			}
		}
	}

	//Set the rounds remaining and health hud components
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("Strafe"), this, &APlayerCharacter::Strafe);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APlayerCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APlayerCharacter::Turn);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &APlayerCharacter::SprintStart);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &APlayerCharacter::SprintEnd);
	PlayerInputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &APlayerCharacter::InteractStart);
	PlayerInputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Released, this, &APlayerCharacter::InteractEnd);
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacter, PlayerRole);
	DOREPLIFETIME(APlayerCharacter, bIsInteracting);
}

void APlayerCharacter::MoveForward(float Value)
{
	if (!bIsInteracting)
	{
		FRotator ForwardRotation = GetControlRotation();
		ForwardRotation.Roll = 0.0f;
		ForwardRotation.Pitch = 0.0f;
		AddMovementInput(ForwardRotation.Vector(), Value);
	}
}

void APlayerCharacter::Strafe(float Value)
{
	if (!bIsInteracting)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void APlayerCharacter::LookUp(float Value)
{
	if (Camera)
	{
		FRotator DeltaRotation = FRotator::ZeroRotator;
		DeltaRotation.Pitch = Value * LookSensitivity;
		//Bonus Task - Removing Stutter by only adding relative rotation if it does not push pitch above or below 90 or -90 respectively
		if (DeltaRotation.Pitch + Camera->RelativeRotation.Pitch < 90.0f && DeltaRotation.Pitch + Camera->RelativeRotation.Pitch > -90.0f)
		{
			Camera->AddRelativeRotation(DeltaRotation);
		}
		//Need to make sure that the camera is not rolling or yawing when the pitch is
		//trying to pitch greater than 90 or less than -90. AddRelativeRotation starts
		//adding things to roll and yaw at these extremes.
		Camera->RelativeRotation.Yaw = 0.0f;
		Camera->RelativeRotation.Roll = 0.0f;
	}
}

void APlayerCharacter::Turn(float Value)
{
	AddControllerYawInput(Value * LookSensitivity);
}

void APlayerCharacter::SprintStart()
{
	if (!bIsInteracting)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintMovementSpeed;
		ServerSprintStart();
	}
}

void APlayerCharacter::SprintEnd()
{
	if (!bIsInteracting)
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalMovementSpeed;
		ServerSprintEnd();
	}
}

void APlayerCharacter::InteractStart()
{
	if (PlayerRole == PlayerRole::HIDER)
	{
		bIsInteracting = true;
		UE_LOG(LogTemp, Warning, TEXT("Interact pressed"))
	}
}

void APlayerCharacter::InteractEnd()
{
	if (PlayerRole == PlayerRole::HIDER)
	{
		bIsInteracting = false;
		UE_LOG(LogTemp, Warning, TEXT("Interact Released"))
	}
}

void APlayerCharacter::OnDeath()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		AMultiplayerGameMode* GameMode = Cast<AMultiplayerGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->Respawn(GetController());
		}
	}

}

void APlayerCharacter::HidePlayerHUD_Implementation(bool bSetHUDVisibility)
{
	//Get the player controller then the player hud of the autonomous proxy
	// CAN ALSO JUST CHECK FOR IsLocallyControlled()
	if (GetLocalRole() == ROLE_AutonomousProxy || (GetLocalRole() == ROLE_Authority && IsLocallyControlled()))
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
		{
			if (APlayerHUD* HUD = Cast<APlayerHUD>(PlayerController->GetHUD()))
			{
				HUD->SetHideWidgets(bSetHUDVisibility);
			}
		}
	}
}

void APlayerCharacter::ServerSprintStart_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintMovementSpeed;
}

void APlayerCharacter::ServerSprintEnd_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = NormalMovementSpeed;
}

