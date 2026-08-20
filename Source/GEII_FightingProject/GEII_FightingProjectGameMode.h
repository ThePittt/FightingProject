// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PlayerHPUI.h"
#include "GEII_FightingProjectGameMode.generated.h"

class AGEII_FightingProjectCharacter;

UCLASS(minimalapi)
class AGEII_FightingProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGEII_FightingProjectGameMode();

	void GameOver();

protected:
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual void BeginPlay() override;

};



