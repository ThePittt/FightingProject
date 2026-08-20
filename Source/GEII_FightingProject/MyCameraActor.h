// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "MyCameraActor.generated.h"

/**
 * 
 */
UCLASS()
class GEII_FIGHTINGPROJECT_API AMyCameraActor : public ACameraActor
{
	GENERATED_BODY()

public:
	void CameraSetup();

	AMyCameraActor();

	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	TSubclassOf<class UPlayerHPUI> MainUIClass;

	UPROPERTY(VisibleAnywhere)
	class UPlayerHPUI* MainHUD;

protected:
	virtual void Tick(float DeltaTime) override;

private:

	AActor* Player1;
	AActor* Player2;

	void Player2EnterySound();

	void StartCombat();

	UPROPERTY(VisibleAnywhere, Category = "Camera Settings")
	float CameraYStartPosition;

	UPROPERTY(VisibleAnywhere, Category = "Camera Settings")
	float BetweenPlayerDistance;

	UPROPERTY(VisibleAnywhere, Category = "Camera Settings")
	float CameraXPosition;

	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	float MinPlayerDistance = 700.0f;

	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	float MaxPlayerDistance = 1500.0f;

	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	float MaxCameraYPullback = 600.0f;

	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	float CameraSmoothing = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Effects")
	class USoundBase* Player1Intro;

	UPROPERTY(EditAnywhere, Category = "Effects")
	class USoundBase* Player2Intro;

	UPROPERTY(EditAnywhere, Category = "Effects")
	class USoundBase* StartFight;
};
