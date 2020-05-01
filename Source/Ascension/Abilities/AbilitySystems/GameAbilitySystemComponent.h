// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameAbilitySystemComponent.generated.h"


/*
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
	/*
	 * Map of ability names to the class of the respective ability.
	 */
	UPROPERTY(Category = Abilities, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<FString, TSubclassOf<UAbility>> AbilitiesMap;

	/*
	 * Map of ability IDs to the current active abilities.
	 */
	UPROPERTY(Category = Abilities, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<uint8, UAbility*> ActiveAbilitiesMap;

	/*
	 * Map of active ability names to their IDs.
	 */
	TMap<FString, TArray<uint8>> ActiveAbilityNameIDsMap;

	/*
	 * Ability system owner.
	 */
	AActor* Owner;

	/*
	 * ID of the next ability.
	 */
	UPROPERTY(Category = Abilities, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	uint8 NextAbilityID;

public:
	/*
	 * Function to get an ability.
	 * @param AbilityName	Name of the ability to get.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Abilities")
	TSubclassOf<UAbility> GetAbility(const FString& AbilityName) const;

	/*
	 * Function to get an active ability.
	 * @param AbilityName	Name of the active ability to get.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Abilities")
	UAbility* GetActiveAbility(const FString& AbilityName, const uint8& AbilityID) const;

	/*
	 * Function to add an ability to the system.
	 * @param AbilityName	Name of ability to add.
	 * @param Ability		Class of ability to add.
	 */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual void AddAbility(const FString& AbilityName, class TSubclassOf<UAbility> Ability);

	/*
	 * Function to clear all abilities in the system.
	 */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual void ClearAbilities();

	/*
	 * Function to check whether the entity can use an ability.
	 * @param AbilityName	Name of ability to check for.
	 */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual bool CanActivateAbility(const FString& AbilityName);

	/*
	 * This method activates an ability. To activate an ability, get the ability class using GetAbility,
	 * instantiate it with the required parameters and use this function to activate the ability.
	 * @param AbilityName			Name of the ability to activate.
	 * @param ActivatedAbilityID	ID of the activated ability.
	 * @param bool					Whether the ability was activated.
	 */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual bool ActivateAbility(const FString& AbilityName, uint8& ActivatedAbilityID);

	/*
	 * This method ends the execution of the current ability.
	 * @param AbilityName	Name of the ability to finish.
	 */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual void FinishAbility(const FString& AbilityName, const uint8& AbilityID);

	/*
	 * Method to print active abilities and their IDs.
	 */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void PrintActiveAbilities() const;

};
