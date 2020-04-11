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
	TMap<FString, TSubclassOf<UAbility>> AbilitiesMap;

	/** Currently active ability. */
	UPROPERTY(Category = Abilities, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<uint32, UAbility*> ActiveAbilitiesMap;

	/** Ability system owner. */
	AActor* Owner;

public:
	/**
	 * Function to get an ability.
	 * @param AbilityName	Name of the ability to get.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Abilities")
	virtual TSubclassOf<UAbility> GetAbility(const FString& AbilityName) const;

	/**
	  * Function to add an ability to the system.
	  * @param AbilityName	Name of ability to add.
	  * @param Ability		Class of ability to add.
	  */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual void AddAbility(const FString& AbilityName, class TSubclassOf<UAbility> Ability);

	/**
	 * Function to clear all abilities in the system.
	 */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual void ClearAbilities();

	/**
	 * Function to check whether the entity can use an ability.
	 * @param AbilityName	Name of ability to check for.
	 */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual bool CanActivateAbility(const FString& AbilityName);

	/**
	  * This method does the setup necessary to activate an ability such as state transitions.
	  * @param AbilityName	Name of the ability to setup.
	  */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual void SetupAbility(const FString& AbilityName);

	/**
	  * This method activates an ability.
	  * @param AbilityName	Name of the ability to activate.
	  * @param Activated	Whether the ability was activated. (Out-Param)
	  * @param AbilityID	ID of the activated ability. (Out-param)
	  */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual void ActivateAbility(const FString& AbilityName, bool& Activated, uint32& AbilityID);

	/**
	  * This method does the actions necessary to finish an ability such as state transitions.
	  * @param AbilityID	ID of the ability to end.
	  */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual void EndAbility(const uint32& AbilityID);

	/**
	  * This method ends the execution of the current ability.
	  * @param AbilityID	ID of the ability to finish.
	  */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual void FinishAbility(const uint32& AbilityID);

};
