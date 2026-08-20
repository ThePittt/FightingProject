// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Logging/LogMacros.h"
#include "PlayerHPUI.h"
#include "GEII_FightingProjectGameMode.h"
#include "GEII_FightingProjectCharacter.generated.h"

class UNiagaraSystem;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AGEII_FightingProjectCharacter : public ACharacter
{
	GENERATED_BODY()

	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;


	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	/** Quick Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* QuickAttackAction;

	/** Heavy Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* HeavyAttackAction;

	/** Special Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SpecialAttackAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	int ControllerIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* DamageSparkEffect;	

	UPROPERTY(EditAnywhere, Category = "Effects")
	class USoundBase* ImpactSound;

	UPlayerHPUI* FightingGameUI;

public:
	AGEII_FightingProjectCharacter();

	void EnablePlayerInput();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void TakeDamageAmmount(int Damage);

	UFUNCTION(BlueprintCallable)
	void ResetCombo();

	// Functions called by notify events to continue combo
	UFUNCTION(BlueprintCallable)
	void ComboBasicAttackSave();

	UFUNCTION(BlueprintCallable)
	void ComboQuickAttackSave();

	UFUNCTION(BlueprintCallable)
	void ComboHeavyAttackSave();

	UFUNCTION(BlueprintCallable)
	void ComboSpecialAttackSave();


	// Functions to activate hitboxes collisions
	UFUNCTION(BlueprintCallable)
	void ActivateCollisionLeft();

	UFUNCTION(BlueprintCallable)
	void ActivateCollisionRight();

	void SetupUI(UPlayerHPUI* InGameUI, int index);

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);	

	/*Function called by input related to basic attacks*/
	void BasicAttack(const FInputActionValue& Value);

	/*Function called by input related to quick attacks*/
	void QuickAttack(const FInputActionValue& Value);

	/*Function called by input related to heavy attacks*/
	void HeavyAttack(const FInputActionValue& Value);

	/*Function called by input related to special attacks*/
	void SpecialAttack(const FInputActionValue& Value);

protected:

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadOnly, Category = "GameMode", meta = (AllowPrivateAcess= "true"))
	AGEII_FightingProjectGameMode* GameMode;

	UPROPERTY(BlueprintReadOnly, Category = "Hitbox", meta = (AllowPrivateAccess = "true"))
	AGEII_FightingProjectCharacter* Enemy;

	/*Character Hurtbox*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* Hurtbox;
	
	/*Character Left Arm Hitbox*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox", meta = (AllowPrivateAccess = "true"))
	USphereComponent* LeftHitbox;

	/*Character Right Arm Hitbox*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox", meta = (AllowPrivateAccess = "true"))
	USphereComponent* RightHitbox;

	/*Count the number of attacks in a combo*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks", meta = (AllowPrivateAccess = "true"))
	int attackCount;

	/*Check if the character is attacking*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks", meta = (AllowPrivateAccess = "true"))
	bool bIsAttacking;

	////////////////////////////////////////////////////////////////////////////////////

	// Variables to check and continue the combo of the attack

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks", meta = (AllowPrivateAccess = "true"))
	bool bIncomingAttackIsBasic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks", meta = (AllowPrivateAccess = "true"))
	bool  bIncomingAttackIsQuick;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks", meta = (AllowPrivateAccess = "true"))
	bool  bIncomingAttackIsHeavy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks", meta = (AllowPrivateAccess = "true"))
	bool  bIncomingAttackIsSpecial;

	////////////////////////////////////////////////////////////////////////////////////

	/*Array for the damage of each basic attack*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	TArray<int> BasicAttacksDamage;

	/*Array for the damage of each quick attack*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	TArray<int> QuickAttacksDamage;

	/*Array for the damage of each heavy attack*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	TArray<int> HeavyAttacksDamage;

	/*Array for the damage of each special attack*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	TArray<int> SpecialAttacksDamage;

	///////////////////////////////////////////////////////////////////

	/*Montages related to basic attacks*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	TArray<UAnimMontage*> BasicAttacksMontages;

	/*Montages related to quick attacks*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	TArray<UAnimMontage*> QuickAttacksMontages;

	/*Montages related to heavy attacks*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	TArray<UAnimMontage*> HeavyAttacksMontages;

	/*Montages related to special attacks*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	TArray<UAnimMontage*> SpecialAttacksMontages;

	/////////////////////////////////////////////////////////////////////

	/*Characters Health*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	int MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	int CurrentHealth;
	
	/*Characters Animation Damage*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	int DamageToDeal;


	/*Characters Animation Damage*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	bool bCanDamage;


private:
	
	/*Function called if it's the first attack of a combo*/
	UFUNCTION()
	void FirstAttack(const TArray<UAnimMontage*>& AnimationArrayToPlay, bool& isAttackSaved, int& AttackDamage);

	/*Function called to continue the combo of an attack*/
	UFUNCTION()
	void ComboAttack(const TArray<UAnimMontage*>& AnimationArrayToPlay, bool& isAttackSaved, TArray<int>& AttackDamage);

	UFUNCTION(BlueprintCallable)
	void OnRightHandOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void OnLeftHandOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool EnableInput;

};

