// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameAbilitySystemComponent.generated.h"


/**
 * A component used for managing abilities tied to an entity.
 */
UCLASS(Blueprintable, ClassGroup=(AbilitySystems), meta=(BlueprintSpawnableComponent) )
class ASCENSION_API UGameAbilitySystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGameAbilitySystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	/** Array of abilities. */
	TArray<class UAbility*> Abilities;

	/** Currently active ability. */
	class UAbility* ActiveAbility;

	/** Ability system owner. */
	AActor* Owner;

public:
	/**
	 * Function to get an ability.
	 * @param AbilityName	Name of the ability to get.
	 */
	virtual UAbility* GetAbility(FString AbilityName);

	/**
	 * Function to initialize an ability.
	 * @param AbilityName	Name of the ability to initialize.
	 */
	virtual void InitializeAbility(FString AbilityName);

	/**
	 * Function to check whether the entity can use an ability.
	 */
	virtual bool CanActivateAbility();

	/**
	 * This method activates an ability.
	 * @param AbilityName	Name of the ability to activate.
	 */
	virtual void ActivateAbility(FString AbilityName);

	/**
	 * This method ends the execution of the current ability.
	 */
	virtual void FinishAbility();

};
