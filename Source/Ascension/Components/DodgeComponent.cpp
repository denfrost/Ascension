// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "DodgeComponent.h"

// Sets default values for this component's properties
UDodgeComponent::UDodgeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDodgeComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACharacter>(GetOwner());
}

bool UDodgeComponent::Dodge_Implementation()
{
	return false;
}

void UDodgeComponent::FinishDodge_Implementation()
{

}
