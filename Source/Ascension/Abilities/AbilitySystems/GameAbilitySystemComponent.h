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
	/**
	  * Map of ability names to the respective ability object.
	  */
	UPROPERTY(Category = Abilities, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<FString, class UAbility*> AbilitiesMap;

	/** Currently active ability. */
	UPROPERTY(Category = Abilities, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<class UAbility*> ActiveAbilities;

	/** Ability system owner. */
	AActor* Owner;

public:
	/**
	 * Function to get an ability.
	 * @param AbilityName	Name of the ability to get.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Abilities")
	virtual UAbility* GetAbility(FString AbilityName) const;

	/**
	 * Function to add an ability to the system.
	 * @param Ability	Ability to add.
	 */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual void AddAbility(class UAbility* Ability);

	/**
	 * Function to clear all abilities in the system.
	 */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual void ClearAbilities();

	/**
	 * Function to initialize an ability.
	 * @param AbilityName	Name of the ability to initialize.
	 */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual void InitializeAbility(FString AbilityName);

	/**
	 * Function to check whether the entity can use an ability.
	 * @param Ability	Ability to check for.
	 */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual bool CanActivateAbility(const UAbility* Ability);

	/**
	 * This method activates an ability.
	 * @param AbilityName	Name of the ability to activate.
	 */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual void ActivateAbility(FString AbilityName);

	/**
	 * This method ends the execution of the current ability.
	 */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual void FinishAbility(FString AbilityName);

};
