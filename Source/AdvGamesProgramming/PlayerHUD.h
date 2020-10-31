// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

UCLASS()
class ADVGAMESPROGRAMMING_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	APlayerHUD();

	void SetPlayerHealthBarPercent(float Percent);

	UFUNCTION(BlueprintCallable)
	void SetAmmoText(int32 RoundsRemaining, int32 MagazineSize);
	void SetRoleText();
	void SetHidingTimerText(int32 TimeLeft);
	void SetTimeLeftTimerText(int32 TimeLeft);


	// Hides or shows the widgets in the hud.
	void SetHideWidgets(bool bIsHidden);

private:

	TSubclassOf<class UUserWidget> PlayerHUDClass;
	UUserWidget* CurrentPlayerHUDWidget;
	class UProgressBar* HealthProgressBar;
	class UTextBlock* AmmoTextBlock;
	class UImage* CrosshairImageBlock;

	class UTextBlock* RoleTextBlock;
	class UTextBlock* TimerTextBlock;

};
