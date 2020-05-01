// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DodgeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASCENSION_API UDodgeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDodgeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	/*
	 * Called for the character to setup the dodge.
	 * @param DodgeName		Name of the dodge to setup.
	 * @param DodgeID		ID of the dodge to setup.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Dodge")
	void SetupDodge(const FString& DodgeName, const uint8 DodgeID);
	virtual void SetupDodge_Implementation(const FString& DodgeName, const uint8 DodgeID);

	/*
	 * Called for the character to perform a dodge.
	 * Dodge name is required in case there are different types of dodges.
	 * @param DodgeName		Name of the dodge to perform. Defaults to "Dodge".
	 * @returns bool		Whether the dodge was executed.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Dodge")
	bool Dodge(const FString& DodgeName);
	virtual bool Dodge_Implementation(const FString& DodgeName);

	/*
	 * Called for the character to finish the dodge.
	 * @param DodgeName		Name of the dodge to finish.
	 * @param DodgeID		ID of the dodge to finish.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Dodge")
	void FinishDodge(const FString& DodgeName, const uint8 DodgeID);
	virtual void FinishDodge_Implementation(const FString& DodgeName, const uint8 DodgeID);

protected:
	/*
	 * Array of active dodge IDs.
	 */
	TArray<uint8> ActiveDodgeIDs;

	/*
	 * Map of active dodge names to IDs.
	 */
	TMap<FString, TArray<uint8>> ActiveDodgeNameIDsMap;

	/** The component's owner. */
	UPROPERTY(VisibleAnywhere, Category = "Owner")
	ACharacter* Owner;
};
