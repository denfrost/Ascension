// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Ascension.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "Globals.h"
#include "AscensionCharacter.h"

//////////////////////////////////////////////////////////////////////////
// AAscensionCharacter

AAscensionCharacter::AAscensionCharacter()
{
	// Set size for collision capsule.
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Set our turn rates for input.
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Set character states.
	CharacterState = ECharacterState::CS_Idle;
	MovementState = EMovementState::MS_OnGround;
	WeaponState = EWeaponState::WS_Sheathed;

	// Set movement speeds.
	WalkSpeed = 200.0f;
	NormalSpeed = 500.0f;
	SprintSpeed = 800.0f;

	// Set gameplay variables.
	ComboMeter = 0;
	CanChain = false;
	ShouldCharSwitch = false;

	// Set player anim structs.
	DodgeAnim = nullptr;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 1080.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AAscensionCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AAscensionCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AAscensionCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AAscensionCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AAscensionCharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AAscensionCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AAscensionCharacter::StopSprinting);

	PlayerInputComponent->BindAction("LightAttack", IE_Pressed, this, &AAscensionCharacter::LightAttack);
	PlayerInputComponent->BindAction("StrongAttack", IE_Pressed, this, &AAscensionCharacter::StrongAttack);

	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &AAscensionCharacter::Dodge);

	PlayerInputComponent->BindAction("SheathWeapon", IE_Pressed, this, &AAscensionCharacter::SwitchWeapon);

	PlayerInputComponent->BindAxis("MoveForward", this, &AAscensionCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AAscensionCharacter::MoveRight);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AAscensionCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AAscensionCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AAscensionCharacter::OnResetVR);
}

void AAscensionCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetCharacterMovement()->IsFalling())
	{
		MovementState = EMovementState::MS_InAir;
	}
	else
	{
		MovementState = EMovementState::MS_OnGround;
	}
}

void AAscensionCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AAscensionCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AAscensionCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}


void AAscensionCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AAscensionCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}


void AAscensionCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AAscensionCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AAscensionCharacter::Sprint()
{
	if (CharacterState == ECharacterState::CS_Idle && MovementState == EMovementState::MS_OnGround)
	{
		UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
		MovementComponent->MaxWalkSpeed = SprintSpeed;
	}
}

void AAscensionCharacter::StopSprinting()
{
	if (CharacterState == ECharacterState::CS_Idle)
	{
		UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
		MovementComponent->MaxWalkSpeed = NormalSpeed;
	}
}

void AAscensionCharacter::Jump()
{
	if (CharacterState == ECharacterState::CS_Idle)
	{
		ACharacter::Jump();
	}
}

void AAscensionCharacter::StopJumping()
{
	if (CharacterState == ECharacterState::CS_Idle)
	{
		ACharacter::StopJumping();
	}
}

void AAscensionCharacter::LightAttack()
{
	if (CanAttack())
	{
		FPlayerAnimation AttackToPerform = SelectAttack(FString("Light Attack"));
		SetLaunchParams(AttackToPerform.LaunchForwardForce, AttackToPerform.LaunchUpwardForce);
		PlayAnimMontage(AttackToPerform.AnimMontage);
		ComboMeter++;
	}
}

void AAscensionCharacter::StrongAttack()
{
	if (CanAttack())
	{
		FPlayerAnimation AttackToPerform = SelectAttack(FString("Strong Attack"));
		SetLaunchParams(AttackToPerform.LaunchForwardForce, AttackToPerform.LaunchUpwardForce);
		PlayAnimMontage(AttackToPerform.AnimMontage);
		ComboMeter++;
	}
}

void AAscensionCharacter::Dodge()
{
	if (CharacterState == ECharacterState::CS_Idle && MovementState == EMovementState::MS_OnGround)
	{
		CharacterState = ECharacterState::CS_Dodging;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		SetLaunchParams(8500.0f, 0.0f);

		StopMovement();
		StopTurning();
		
		if (DodgeAnim != nullptr)
		{
			PlayAnimMontage(DodgeAnim);
		}
	}
}

void AAscensionCharacter::SwitchWeapon()
{
	if (CharacterState == ECharacterState::CS_Idle && MovementState == EMovementState::MS_OnGround)
	{
		switch (WeaponState)
		{
		case EWeaponState::WS_Sheathed:
			WeaponState = EWeaponState::WS_Unsheathed;
			break;

		case EWeaponState::WS_Unsheathed:
			WeaponState = EWeaponState::WS_Sheathed;
			break;
		}

		StopMovement();
		CharacterState = ECharacterState::CS_Switching;
		ShouldCharSwitch = true;
		LimitTurning(100.0f);
	}
}

bool AAscensionCharacter::CanAttack()
{
	if ((CharacterState == ECharacterState::CS_Idle || CharacterState == ECharacterState::CS_Attacking) &&
		(MovementState == EMovementState::MS_OnGround) && (WeaponState == EWeaponState::WS_Unsheathed))
	{
		if (CharacterState == ECharacterState::CS_Attacking)
		{
			if (CanChain)
			{
				CanChain = false;
				StopMovement();
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			CharacterState = ECharacterState::CS_Attacking;
			StopMovement();
			return true;
		}
	}
	else
	{
		return false;
	}
}

void AAscensionCharacter::StopMovement()
{
	GetCharacterMovement()->MaxWalkSpeed = 0.0f;
}

void AAscensionCharacter::LimitMovement(float Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

void AAscensionCharacter::ResetMovement()
{
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
}

void AAscensionCharacter::StopTurning()
{
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f);
}

void AAscensionCharacter::LimitTurning(float Rate)
{
	GetCharacterMovement()->RotationRate = FRotator(0.0f, Rate, 0.0f);
}

void AAscensionCharacter::ResetTurning()
{
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 1080.0f, 0.0f);
}

void AAscensionCharacter::SetGravity(float GravityValue)
{
	GetCharacterMovement()->GravityScale = GravityValue;
}

void AAscensionCharacter::ResetGravity()
{
	GetCharacterMovement()->GravityScale = 1.0f;
}

void AAscensionCharacter::SetLaunchParams(float LaunchHForce, float LaunchVForce)
{
	LaunchForwardForce = LaunchHForce;
	LaunchUpwardForce = LaunchVForce;
}

FPlayerAnimation AAscensionCharacter::SelectAttack(FString AttackType)
{
	if (AttackType.Equals(FString("Light Attack")))
	{
		switch (ComboMeter)
		{
		case 0:
			return LightAttack01;
			break;

		case 1:
			return LightAttack02;
			break;

		case 2:
			return LightAttack03;
			break;
		}
	}
	
	else if (AttackType.Equals(FString("Strong Attack")))
	{
		switch (ComboMeter)
		{
		case 0:
			return StrongAttack01;
			break;

		case 1:
			return StrongAttack02;
			break;

		case 2:
			return StrongAttack03;
			break;

		case 3:
			return StrongAttack04;
			break;
		}
	}

	return LightAttack01;
}

void AAscensionCharacter::SwitchComplete_Implementation()
{
	CharacterState = ECharacterState::CS_Idle;
	ShouldCharSwitch = false;
	ResetMovement();
	ResetTurning();
}

void AAscensionCharacter::ResetCombo_Implementation()
{
	CharacterState = ECharacterState::CS_Idle;
	ComboMeter = 0;
	CanChain = false;
	ResetGravity();
	ResetMovement();
}

void AAscensionCharacter::ResetDodge_Implementation()
{
	CharacterState = ECharacterState::CS_Idle;
	ResetMovement();
	ResetTurning();
	ResetGravity();
}

void AAscensionCharacter::LimitTurn_Implementation()
{
	StopTurning();
}

void AAscensionCharacter::ResetTurn_Implementation()
{
	ResetTurning();
}

void AAscensionCharacter::LaunchChar_Implementation()
{
	SetGravity(100.0f);
	FVector LaunchVeloctiy = (GetActorForwardVector() * LaunchForwardForce) + (GetActorUpVector() * LaunchUpwardForce);
	LaunchCharacter(LaunchVeloctiy, true, false);
}

void AAscensionCharacter::CanChainAttack_Implementation()
{
	CanChain = true;
}

void AAscensionCharacter::SetFlyable_Implementation()
{
	GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Flying;
}

void AAscensionCharacter::ResetFlyable_Implementation()
{
	GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;
}

void AAscensionCharacter::Sheathed_Implementation() {}
void AAscensionCharacter::Unsheathed_Implementation() {}
void AAscensionCharacter::FootstepSound_Implementation() {}
void AAscensionCharacter::SlashSound_Implementation() {}
