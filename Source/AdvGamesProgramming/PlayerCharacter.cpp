// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "HealthComponent.h"
#include "MultiplayerGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "PlayerHUD.h"
#include "Engine/Engine.h"


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

    bIsDead = false;
    HealDelay = 0.3f;
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

    // If being healed, heal
    if (bIsPlayerHealing && bIsDead)
    {
        if (HealTimer <= 0)
        {
            Heal();
            HealthComponent->UpdateHealthBar();
        }
    }

    // Timer
    if (HealTimer > 0)
    {
        HealTimer -= GetWorld()->GetDeltaSeconds();
    }

    if (bIsDead)
    {
        if (HealthComponent->HealthPercentageRemaining() >= 1.0f)
        {
            bIsDead = false;
            bIsPlayerHealing = false;
            //EnableInput(Cast<APlayerController>(GetController()));
        }
    }
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
	PlayerInputComponent->BindAction(TEXT("StartGame"), EInputEvent::IE_Pressed, this, &APlayerCharacter::StartGame);

}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(APlayerCharacter, PlayerRole);
    DOREPLIFETIME(APlayerCharacter, bIsInteracting);
    DOREPLIFETIME(APlayerCharacter, bIsDead);
    DOREPLIFETIME(APlayerCharacter, bIsPlayerHealing);
}

void APlayerCharacter::MoveForward(float Value)
{
    if (!bIsInteracting && !bIsDead)
    {
        FRotator ForwardRotation = GetControlRotation();
        ForwardRotation.Roll = 0.0f;
        ForwardRotation.Pitch = 0.0f;
        AddMovementInput(ForwardRotation.Vector(), Value);
    }
}

void APlayerCharacter::Strafe(float Value)
{
    if (!bIsInteracting && !bIsDead)
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
        if (DeltaRotation.Pitch + Camera->RelativeRotation.Pitch < 90.0f && DeltaRotation.Pitch + Camera->
            RelativeRotation.Pitch > -90.0f)
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
    if (!bIsInteracting && !bIsDead)
    {
        GetCharacterMovement()->MaxWalkSpeed = SprintMovementSpeed;
        ServerSprintStart();
    }
}

void APlayerCharacter::SprintEnd()
{
    if (!bIsInteracting && !bIsDead)
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
        HealHiders();
        UE_LOG(LogTemp, Warning, TEXT("Interact pressed"))
    }
}

void APlayerCharacter::InteractEnd()
{
    if (PlayerRole == PlayerRole::HIDER)
    {
        bIsInteracting = false;
        HealHiders_END();
        UE_LOG(LogTemp, Warning, TEXT("Interact Released"))
    }
}

// Start Game can only be called if is Host
void APlayerCharacter::StartGame()
{
	if (!bGameStarted)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enter pressed for StartGame."))
		AMultiplayerGameMode* GameMode = Cast<AMultiplayerGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			bGameStarted = true;
			GameMode->StartGame();
			
		}
	}
}

void APlayerCharacter::OnDeath()
{
    // if (IsLocallyControlled())
    // {
    AMultiplayerGameMode* GameMode = Cast<AMultiplayerGameMode>(GetWorld()->GetAuthGameMode());
    if (GameMode)
    {
        GameMode->PlayerDied(GetController());
    }

    bIsDead = true;
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("Player is %hs"), bIsDead? "true" : "false"));
    }
    // DisableInput(Cast<APlayerController>(GetController()));
    // UE_LOG(LogTemp, Warning, TEXT("Died"))
    // }
}

void APlayerCharacter::Heal()
{
    HealthComponent->CurrentHealth += 3.0f;
    FMath::Clamp(HealthComponent->CurrentHealth, 0.0f, 100.0f);
    HealTimer = HealDelay;
    UE_LOG(LogTemp, Display, TEXT("Healed"));
}

void APlayerCharacter::SetSeeker()
{
	UE_LOG(LogTemp, Warning, TEXT("Setting Seeker!"))
	SetSeekerMaterial(true);
	//Set HUD
	if (GetLocalRole() == ROLE_AutonomousProxy || (GetLocalRole() == ROLE_Authority && IsLocallyControlled()))
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
		{
			if (APlayerHUD* HUD = Cast<APlayerHUD>(PlayerController->GetHUD()))
			{
				HUD->SetRoleText(TEXT("Seeker"));
				HUD->SetHideNumPlayersText(true);
				HUD->SetHideStartGameButton(true);
				bIsInteracting = true;
			}
		}
	}
}

void APlayerCharacter::SetHider()
{
	UE_LOG(LogTemp, Warning, TEXT("Setting Hider!"))
	SetSeekerMaterial(false);
	//Set HUD
	if (GetLocalRole() == ROLE_AutonomousProxy || (GetLocalRole() == ROLE_Authority && IsLocallyControlled()))
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
		{
			if (APlayerHUD* HUD = Cast<APlayerHUD>(PlayerController->GetHUD()))
			{
				HUD->SetRoleText(TEXT("Hider"));
				HUD->SetHideNumPlayersText(true);
				HUD->SetHideStartGameButton(true);
			}
		}
	}
}


void APlayerCharacter::SetHidingTimerHUD(int32 TimeLeft)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		SetCanFire(false);
		if (APlayerHUD* HUD = Cast<APlayerHUD>(PlayerController->GetHUD()))
		{
			HUD->SetHideTimerText(false);
			HUD->SetHidingTimerText(TimeLeft);
			UE_LOG(LogTemp, Warning, TEXT("Hiding time left: %i"), TimeLeft)
		}
	}
}

void APlayerCharacter::SetSeekingTimerHUD(int32 TimeLeft)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		SetCanFire(true);
		bIsInteracting = false;
		if (APlayerHUD* HUD = Cast<APlayerHUD>(PlayerController->GetHUD()))
		{
			HUD->SetTimeLeftTimerText(TimeLeft);
			UE_LOG(LogTemp, Warning, TEXT("Hiding time left: %i"), TimeLeft)
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
