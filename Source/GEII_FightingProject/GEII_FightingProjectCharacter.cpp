// Copyright Epic Games, Inc. All Rights Reserved.

#include "GEII_FightingProjectCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TimerManager.h"
#include "PlayerHPUI.h"
#include "InputActionValue.h"
#include <Kismet/GameplayStatics.h>

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AGEII_FightingProjectCharacter

AGEII_FightingProjectCharacter::AGEII_FightingProjectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;


	// Variables FName corresponding to the sockets
	FName RightArmSocket = TEXT("Hand_RSocket");
	FName LeftArmSocket = TEXT("Hand_LSocket");
	FName ChestSocket = TEXT("Chest");

	//Create and attach hurtbox to root component
	Hurtbox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BodyHurtbox"));
	Hurtbox->SetupAttachment(GetMesh(), FName("Chest"));

	//Create and attach hitbox to left arm socket
	LeftHitbox = CreateDefaultSubobject<USphereComponent>(TEXT("LeftArmHitbox"));
	LeftHitbox->SetupAttachment(GetMesh(), FName("Hand_LSocket"));
	LeftHitbox->InitSphereRadius(50);

	//Create and attach hitbox to right arm socket
	RightHitbox = CreateDefaultSubobject<USphereComponent>(TEXT("RightArmHitbox"));
	RightHitbox->SetupAttachment(GetMesh(), FName("Hand_RSocket"));
	RightHitbox->InitSphereRadius(50);

	ImpactSound = CreateDefaultSubobject<USoundBase>(TEXT("Impact Sound"));


	bIsAttacking = false;
	attackCount = 0;
	MaxHealth = 100;
	CurrentHealth = MaxHealth;
	bCanDamage = false;
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AGEII_FightingProjectCharacter::SetupUI(UPlayerHPUI* InGameUI, int index)
{
	ControllerIndex = index;
	FightingGameUI = InGameUI;
	CurrentHealth = MaxHealth;

	if (ControllerIndex >= 1)
	{
		InGameUI->SetPlayer2Health(CurrentHealth, MaxHealth);
	}
	else
	{
		InGameUI->SetPlayer1Health(CurrentHealth, MaxHealth);
	}

	// Set EnableInput to false initially
	EnableInput = false;

	FTimerHandle EnableInputTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(EnableInputTimerHandle,FTimerDelegate::CreateUObject(this, &AGEII_FightingProjectCharacter::EnablePlayerInput), 6.5f, false);
}

void AGEII_FightingProjectCharacter::EnablePlayerInput()
{
	EnableInput = true;
	FightingGameUI->StartCombat();
}



void AGEII_FightingProjectCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Assigns the hitboxes to the function that work as overlapping events
	LeftHitbox->OnComponentBeginOverlap.AddDynamic(this, &AGEII_FightingProjectCharacter::OnLeftHandOverlapBegin);
	RightHitbox->OnComponentBeginOverlap.AddDynamic(this, &AGEII_FightingProjectCharacter::OnRightHandOverlapBegin);

	//Disable colllision for hitboxes
	LeftHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

/* Reset Combo*/
void AGEII_FightingProjectCharacter::ResetCombo()
{
	attackCount = 0;
	DamageToDeal = 0;
	bIsAttacking = false;
	bIncomingAttackIsBasic = false;
	bIncomingAttackIsHeavy = false;
	bIncomingAttackIsQuick = false;
	bIncomingAttackIsSpecial = false;
	bCanDamage = false;
}
/*Activates Right Arm Collision*/
void AGEII_FightingProjectCharacter::ActivateCollisionRight()
{
	bCanDamage = true;
	RightHitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

/*Activates Left Arm Collision*/
void AGEII_FightingProjectCharacter::ActivateCollisionLeft()
{
	bCanDamage = true;
	LeftHitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}
void AGEII_FightingProjectCharacter::OnRightHandOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this && bCanDamage)
	{
		if (Enemy == nullptr)
		{
			Enemy = Cast <AGEII_FightingProjectCharacter>(OtherActor);
		}
		bCanDamage = false;
		Enemy->TakeDamageAmmount(DamageToDeal);
	}
}

void AGEII_FightingProjectCharacter::OnLeftHandOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this && bCanDamage)
	{
		if (Enemy == nullptr)
		{
			Enemy = Cast <AGEII_FightingProjectCharacter>(OtherActor);
		}
		bCanDamage = false;
		Enemy->TakeDamageAmmount(DamageToDeal);
	}
}


//Notify Event calls function to continue Basic combo
void AGEII_FightingProjectCharacter::ComboBasicAttackSave()
{
	LeftHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ComboAttack(BasicAttacksMontages, bIncomingAttackIsBasic, BasicAttacksDamage);
}
//Notify Event calls function to continue Quick combo
void AGEII_FightingProjectCharacter::ComboQuickAttackSave()
{
	LeftHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ComboAttack(QuickAttacksMontages, bIncomingAttackIsQuick, QuickAttacksDamage);
}
//Notify Event calls function to continue Heavy combo
void AGEII_FightingProjectCharacter::ComboHeavyAttackSave()
{
	LeftHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ComboAttack(HeavyAttacksMontages, bIncomingAttackIsHeavy, HeavyAttacksDamage);
}
//Notify Event calls function to continue Special combo
void AGEII_FightingProjectCharacter::ComboSpecialAttackSave()
{
	LeftHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ComboAttack(SpecialAttacksMontages, bIncomingAttackIsSpecial, SpecialAttacksDamage);
}

void AGEII_FightingProjectCharacter::FirstAttack(const TArray<UAnimMontage*>& AnimationArrayToPlay, bool& isAttackSaved, int& AttackDamage)
{
	//If its the first attack and isnt attack does the first
	if (!bIsAttacking)
	{
		bIsAttacking = true;

		if (IsValid(AnimationArrayToPlay[attackCount]))
		{
			PlayAnimMontage(AnimationArrayToPlay[attackCount]);

		}
		DamageToDeal = AttackDamage;
		attackCount = 1;
	}
	//Set variable of the type of attack to true if it was false
	else if (!isAttackSaved)
	{
		isAttackSaved = true;
	}
}

void AGEII_FightingProjectCharacter::ComboAttack(const TArray<UAnimMontage*>& AnimationArrayToPlay, bool& isAttackSaved, TArray<int>& AttackDamage)
{
	if (isAttackSaved)
	{
		isAttackSaved = false;

		PlayAnimMontage(AnimationArrayToPlay[attackCount]);
		DamageToDeal = AttackDamage[attackCount];

		//If number of attack exceeds number of animation reset attacount and set variable of is attacking to false to give  a small interval
		if ((attackCount + 1) > (AnimationArrayToPlay.Num() - 1))
		{
			attackCount = 0;
			bIsAttacking = false;
		}
		else
		{
			attackCount++;
		}
	}

}
/* Receives damage according to animation and reduce health by ammount*/
void AGEII_FightingProjectCharacter::TakeDamageAmmount(int Damage)
{
	CurrentHealth = CurrentHealth - Damage;

	FVector SpawnLocation = GetActorLocation();

	FRotator SparkRotation(0.0f, 180.0f, 0.0f);

	UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		DamageSparkEffect,
		SpawnLocation,
		SparkRotation,  
		FVector(1.0f),
		true
	);

	UGameplayStatics::PlaySound2D(GetWorld(), ImpactSound,.5f);

	if (ControllerIndex >= 1)
	{
		FightingGameUI->UPlayerHPUI::SetPlayer2Health(CurrentHealth, MaxHealth);
	}
	else
	{
		FightingGameUI->UPlayerHPUI::SetPlayer1Health(CurrentHealth, MaxHealth);
	}

	if (CurrentHealth <= 0)
	{
		if (ControllerIndex >= 1)
		{
			FightingGameUI->UPlayerHPUI::UPlayerHPUI::VictoryPlayer1();
		}
		else
		{
			FightingGameUI->UPlayerHPUI::UPlayerHPUI::VictoryPlayer2();
		}
		GameMode = Cast<AGEII_FightingProjectGameMode>(GetWorld()->GetAuthGameMode());
		GameMode->GameOver();
	}
}

/* Function Triggered by Basic Attack Input*/
void AGEII_FightingProjectCharacter::BasicAttack(const FInputActionValue& Value)
{
	if (EnableInput)
	{
		FirstAttack(BasicAttacksMontages, bIncomingAttackIsBasic, BasicAttacksDamage[1]);
	}

}
/* Function Triggered by Quick Attack Input*/
void AGEII_FightingProjectCharacter::QuickAttack(const FInputActionValue& Value)
{
	if (EnableInput)
	{
		FirstAttack(QuickAttacksMontages, bIncomingAttackIsQuick, QuickAttacksDamage[1]);

	}
}

/* Function Triggered by Heavy Attack Input*/
void AGEII_FightingProjectCharacter::HeavyAttack(const FInputActionValue& Value)
{
	if (EnableInput)
	{
		FirstAttack(HeavyAttacksMontages, bIncomingAttackIsHeavy, HeavyAttacksDamage[1]);
	}
}

/* Function Triggered by Special Attack Input*/
void AGEII_FightingProjectCharacter::SpecialAttack(const FInputActionValue& Value)
{
	if (EnableInput)
	{
		FirstAttack(SpecialAttacksMontages, bIncomingAttackIsSpecial, SpecialAttacksDamage[1]);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGEII_FightingProjectCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AGEII_FightingProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {


		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGEII_FightingProjectCharacter::Move);

		//Basic Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AGEII_FightingProjectCharacter::BasicAttack);
		//Quick Attack
		EnhancedInputComponent->BindAction(QuickAttackAction, ETriggerEvent::Triggered, this, &AGEII_FightingProjectCharacter::QuickAttack);
		//Heavy Attack
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Triggered, this, &AGEII_FightingProjectCharacter::HeavyAttack);
		//Special Attack
		EnhancedInputComponent->BindAction(SpecialAttackAction, ETriggerEvent::Triggered, this, &AGEII_FightingProjectCharacter::SpecialAttack);

	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AGEII_FightingProjectCharacter::Move(const FInputActionValue& Value)
{
	if (EnableInput)
	{
		// input is a Vector2D
		FVector2D MovementVector = Value.Get<FVector2D>();

		if (Controller != nullptr)
		{
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			if (ControllerIndex>=1)
			{
				const FVector ForwardDirection = ((FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X))*(-1));
				// add movement 
				if (MovementVector < FVector2D(0, 0))
				{
					AddMovementInput(ForwardDirection, MovementVector.Y);
	
				}
				else 
				{
					FVector2D BackwardsMovement = (MovementVector * .5f);
					AddMovementInput(ForwardDirection, BackwardsMovement.Y);
				}
			}
			else
			{
				const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
				if (MovementVector < FVector2D(0, 0))
				{
					AddMovementInput(ForwardDirection, MovementVector.Y);

				}
				else
				{
					FVector2D BackwardsMovement = (MovementVector * .5f);
					AddMovementInput(ForwardDirection, BackwardsMovement.Y);
				}
			}
		
			// get right vector 
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			


		}
	}
	
}



