// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "Components/PlayerInputComponent.h"
#include "AscensionPlayerController.h"


FName AAscensionPlayerController::PlayerInputComponentName(TEXT("PlayerInputComponent"));

AAscensionPlayerController::AAscensionPlayerController()
{
	PlayerInputComponent = CreateDefaultSubobject<UPlayerInputComponent>(PlayerInputComponentName);
}
