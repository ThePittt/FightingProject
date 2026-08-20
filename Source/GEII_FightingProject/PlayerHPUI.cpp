// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHPUI.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Widget.h"
#include "TimerManager.h"

void UPlayerHPUI::SetPlayer1Health(float CurrentPlayer1Health, float MaxPlayer1Health)
{
	if (Player1HealthBar)
	{
		Player1HealthBar->SetPercent(CurrentPlayer1Health / MaxPlayer1Health);
	}
}

void UPlayerHPUI::SetPlayer2Health(float CurrentPlayer2Health, float MaxPlayer2Health)
{
	if (Player2HealthBar)
	{
		Player2HealthBar->SetPercent(CurrentPlayer2Health / MaxPlayer2Health);
	}
}

void UPlayerHPUI::StartCombat()
{
    Start->SetVisibility(ESlateVisibility::Visible);

    FTimerHandle HideStartWidgetTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(HideStartWidgetTimerHandle, FTimerDelegate::CreateUObject(this, &UPlayerHPUI::HideStartWidget), 2.0f, false);
}

void UPlayerHPUI::HideStartWidget()
{
    Start->SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerHPUI::VictoryPlayer1()
{
	Player1Wins->UWidget::SetVisibility(ESlateVisibility::Visible);
	Player2Wins->UWidget::SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerHPUI::VictoryPlayer2()
{
	Player2Wins->UWidget::SetVisibility(ESlateVisibility::Visible);
	Player1Wins->UWidget::SetVisibility(ESlateVisibility::Hidden);
}

