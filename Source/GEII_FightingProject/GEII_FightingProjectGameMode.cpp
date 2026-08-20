// Copyright Epic Games, Inc. All Rights Reserved.

#include "GEII_FightingProjectGameMode.h"
#include "GEII_FightingProjectCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "MyCameraActor.h"
#include "GameFramework/PlayerStart.h"
#include "UObject/ConstructorHelpers.h"


AGEII_FightingProjectGameMode::AGEII_FightingProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}



AActor* AGEII_FightingProjectGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	UWorld* CurrentWorld = GetWorld();
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(CurrentWorld, APlayerStart::StaticClass(), PlayerStarts);

	if (PlayerStarts.Num() < 2)
	{
		return Super::ChoosePlayerStart_Implementation(Player);
	}

	APlayerController* PlayerController = Cast<APlayerController>(Player);

	if (!PlayerController)
	{
		return Super::ChoosePlayerStart_Implementation(Player);
	}

	int32 PlayerIndex = GetWorld()->GetFirstPlayerController() == PlayerController ? 0 : 1;
	FName DesiredTag;

	if (PlayerIndex == 0)
	{
		DesiredTag = FName("Player1");
	}
	else
	{
		DesiredTag = FName("Player2");
	}

	for (AActor* Start : PlayerStarts)
	{
		if (Start->Tags.Contains(DesiredTag))
		{
			return Start;
		}
	}

	if (PlayerStarts.Num() >= 2)
	{
		return PlayerStarts[PlayerIndex % PlayerStarts.Num()];
	}

	return Super::ChoosePlayerStart_Implementation(Player);
}

void AGEII_FightingProjectGameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Setup text"));

	UWorld* CurrentWorld = GetWorld();
	UGameInstance* GameInstance = CurrentWorld->GetGameInstance();
	FString Error;
	ULocalPlayer* localPlayer = GameInstance->CreateLocalPlayer(1, Error, true);

	APlayerController* Player1Controller;
	Player1Controller = UGameplayStatics::GetPlayerController(CurrentWorld, 0);

	AActor* InGameCamera = UGameplayStatics::GetActorOfClass(CurrentWorld, AMyCameraActor::StaticClass());

	Player1Controller->SetViewTargetWithBlend(InGameCamera, 0);

	AMyCameraActor* FightingCamera = Cast<AMyCameraActor>(InGameCamera);

	if (IsValid(FightingCamera))
	{
		FightingCamera->CameraSetup();
	}
}
void AGEII_FightingProjectGameMode::GameOver()
{

	APlayerController* Player1Controller;
	Player1Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	Player1Controller->SetPause(true);
}
