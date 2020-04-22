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
	 * Called for the character to perform a dodge.
	 * @returns bool	Whether the dodge was executed.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Dodge")
	bool Dodge();
	virtual bool Dodge_Implementation();

	/*
	 * Called for the character to finish the dodge.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Dodge")
	void FinishDodge();
	virtual void FinishDodge_Implementation();

protected:
	/** The component's owner. */
	UPROPERTY(VisibleAnywhere, Category = "Owner")
	ACharacter* Owner;
	
};
