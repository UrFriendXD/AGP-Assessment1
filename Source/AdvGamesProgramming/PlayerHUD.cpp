// Fill out your copyright notice in the Description page of Project Settings.
#include "PlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "PlayerCharacter.h"

APlayerHUD::APlayerHUD()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> PlayerHUDObject(TEXT("/Game/Widgets/PlayerHUDWidget"));

	PlayerHUDClass = PlayerHUDObject.Class;

	//Make sure the PlayerHUD class was found correctly
	if (PlayerHUDClass)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerHUD was found."))
		//Need to check that the widget was created successfully
		CurrentPlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDClass);
		if (CurrentPlayerHUDWidget)
		{
			UE_LOG(LogTemp, Error, TEXT("Widget was created successfully."))
			//Draw the hud to the player controllers viewport
			CurrentPlayerHUDWidget->AddToViewport();
			//Find the health bar and the ammo text block
			HealthProgressBar = Cast<UProgressBar>(CurrentPlayerHUDWidget->GetWidgetFromName(TEXT("ProgHealthBar")));
			AmmoTextBlock = Cast<UTextBlock>(CurrentPlayerHUDWidget->GetWidgetFromName(TEXT("TextAmmo")));
			CrosshairImageBlock = Cast<UImage>(CurrentPlayerHUDWidget->GetWidgetFromName(TEXT("ImgCrosshair")));
			RoleTextBlock = Cast<UTextBlock>(CurrentPlayerHUDWidget->GetWidgetFromName(TEXT("TextRole")));
			TimerTextBlock = Cast<UTextBlock>(CurrentPlayerHUDWidget->GetWidgetFromName(TEXT("TextTimer")));
			StartGameButton = Cast<UButton>(CurrentPlayerHUDWidget->GetWidgetFromName(TEXT("StartGameButton")));

		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Widget was not created."))
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerHUD was NOT found."))
	}
}

void APlayerHUD::SetPlayerHealthBarPercent(float Percent)
{
	if (HealthProgressBar) {
		HealthProgressBar->SetPercent(Percent);
	}
}

void APlayerHUD::SetAmmoText(int32 RoundsRemaining, int32 MagazineSize)
{
	if (AmmoTextBlock)
	{
		AmmoTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%i/%i"), RoundsRemaining, MagazineSize)));
	}
}

void APlayerHUD::SetRoleText(FString Role)
{
	if (RoleTextBlock)
	{
		//RoleTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%s"), *Role)));

	}
}

void APlayerHUD::SetNumPlayersText(int32 NumPlayers)
{
	if (NumPlayersTextBlock)
	{
		NumPlayersTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%i/4 Players"), NumPlayers)));

	}
}

void APlayerHUD::SetWaitingForHostTimerText()
{
	if (TimerTextBlock)
	{
		TimerTextBlock->SetText(FText::FromString(FString::Printf(TEXT("Waiting for host to start..."))));
	}
}

void APlayerHUD::SetHidingTimerText(int32 TimeLeft)
{
	if (TimerTextBlock)
	{
		TimerTextBlock->SetText(FText::FromString(FString::Printf(TEXT("Hiding: %is"), TimeLeft)));
	}
}

void APlayerHUD::SetTimeLeftTimerText(int32 TimeLeft)
{
	if (TimerTextBlock)
	{
		TimerTextBlock->SetText(FText::FromString(FString::Printf(TEXT("Time Left: %is"), TimeLeft)));
	}
}

void APlayerHUD::SetHideTimerText(bool bIsHidden)
{
	if (TimerTextBlock)
	{
		if (bIsHidden)
		{
			TimerTextBlock->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			TimerTextBlock->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void APlayerHUD::SetHideStartGameButton(bool bIsHidden)
{
	if (StartGameButton)
	{

		if (bIsHidden)
		{
			StartGameButton->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			StartGameButton->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void APlayerHUD::SetHideWidgets(bool bIsHidden)
{
	if (bIsHidden)
	{
		if (HealthProgressBar) 
			HealthProgressBar->SetVisibility(ESlateVisibility::Hidden);
		if (AmmoTextBlock)
			AmmoTextBlock->SetVisibility(ESlateVisibility::Hidden);
		if (CrosshairImageBlock)
			CrosshairImageBlock->SetVisibility(ESlateVisibility::Hidden);
		// Don't want to hide RoleText, TimerText, NumPlayersText, or StartButton
	}
	else
	{
		if (HealthProgressBar)
		{
			HealthProgressBar->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Something went wrong with the hud."));
		}
			
		if (AmmoTextBlock)
			AmmoTextBlock->SetVisibility(ESlateVisibility::Visible);
		if (CrosshairImageBlock)
			CrosshairImageBlock->SetVisibility(ESlateVisibility::Visible);
		//Hack way. Having Gun C++ class instead of blueprints could fix this issue
		SetAmmoText(15, 15);
		SetPlayerHealthBarPercent(1.0f);
	}
}


