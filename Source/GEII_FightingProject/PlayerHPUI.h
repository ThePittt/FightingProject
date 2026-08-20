// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHPUI.generated.h"

/**
 * 
 */
UCLASS()
class GEII_FIGHTINGPROJECT_API UPlayerHPUI : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void SetPlayer1Health(float CurrentPlayer1Health, float MaxPlayer1Health);
	void SetPlayer2Health(float CurrentPlayer2Health, float MaxPlayer2Health);

	void StartCombat();

	void HideStartWidget();

	void VictoryPlayer1();
	void VictoryPlayer2();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* Player1HealthBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* Player2HealthBar;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Player1Wins;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Player2Wins;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Start;
};
