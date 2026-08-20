// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerHPUI.h"
#include "Blueprint/UserWidget.h"
#include "GEII_FightingProjectCharacter.h"
#include "GameFramework/Character.h"

AMyCameraActor::AMyCameraActor()
{
	PrimaryActorTick.bCanEverTick = true;
	UE_LOG(LogTemp, Warning, TEXT("BBBB"));
	Player1Intro = CreateDefaultSubobject<USoundBase>(TEXT("Player1Intro"));
	Player2Intro = CreateDefaultSubobject<USoundBase>(TEXT("Player2Intro"));
}

void AMyCameraActor::CameraSetup()
{
	UE_LOG(LogTemp, Warning, TEXT("AbleToCast"));

	FVector CameraPosition = GetActorLocation();
	CameraYStartPosition = CameraPosition.Y;

	Player1 = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Player2 = UGameplayStatics::GetPlayerCharacter(GetWorld(), 1);

	if (IsValid(Player1) && IsValid(Player2))
	{
		FVector Player1Position = Player1->GetActorLocation();
		FVector Player2Position = Player2->GetActorLocation();
		BetweenPlayerDistance = FMath::Abs(Player1Position.X - Player2Position.X);
	}
	APlayerController* Player1Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	MainHUD = CreateWidget<UPlayerHPUI>(Player1Controller, MainUIClass);
	MainHUD->AddToViewport();

	AGEII_FightingProjectCharacter* Player1Casted = Cast<AGEII_FightingProjectCharacter>(Player1);
	AGEII_FightingProjectCharacter* Player2Casted = Cast<AGEII_FightingProjectCharacter>(Player2);

	Player1Casted->SetupUI(MainHUD, 0);
	Player2Casted->SetupUI(MainHUD, 1);

	UGameplayStatics::PlaySound2D(GetWorld(), Player1Intro, 1.f);
	FTimerHandle EnableInputTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(EnableInputTimerHandle, FTimerDelegate::CreateUObject(this, &AMyCameraActor::Player2EnterySound), 2.7f, false);
}
void AMyCameraActor::Player2EnterySound()
{
	UGameplayStatics::PlaySound2D(GetWorld(), Player2Intro, 1.f);
	FTimerHandle EnableInputTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(EnableInputTimerHandle, FTimerDelegate::CreateUObject(this, &AMyCameraActor::StartCombat), 3.8f, false);
}

void AMyCameraActor::StartCombat()
{
	UGameplayStatics::PlaySound2D(GetWorld(), StartFight, 1.2f);
}

void AMyCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(Player1) && IsValid(Player2))
	{

		FVector Player1Position = Player1->GetActorLocation();
		FVector Player2Position = Player2->GetActorLocation();

		FVector Midpoint = (Player1Position + Player2Position) / 2.0f;

		float CurrentPlayerDistance = FMath::Abs(Player1Position.X - Player2Position.X);

		FVector CameraPosition = GetActorLocation();

		float TargetYPosition = CameraYStartPosition;

		if (CurrentPlayerDistance > MinPlayerDistance)
		{
			// Calculate how much to pull back the camera (clamped between min and max distances)
			float DistanceFactor = FMath::Clamp((CurrentPlayerDistance - MinPlayerDistance) / (MaxPlayerDistance - MinPlayerDistance), 0.0f, 1.0f);

			// Calculate the target Y position with pullback
			TargetYPosition = CameraYStartPosition - (MaxCameraYPullback * DistanceFactor);
		}

		float NewY = FMath::Lerp(CameraPosition.Y, TargetYPosition, CameraSmoothing);

		float NewX = Midpoint.X;
		SetActorLocation(FVector(NewX, NewY, CameraPosition.Z));
	}
}

