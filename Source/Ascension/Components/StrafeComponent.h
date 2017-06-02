// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "StrafeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASCENSION_API UStrafeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStrafeComponent();

	/** Called when the component comes into play. */
	void BeginPlay();

	/** Function called for character to strafe around an enemy. */
	UFUNCTION(BlueprintCallable, Category = "Strafe")
	void Strafe(AActor* Enemy);

private:
	UPROPERTY(VisibleAnywhere, Category = "Owner")
	ACharacter* Owner;

	UPROPERTY(VisibleAnywhere, Category = "Owner")
	AController* Controller;
};
