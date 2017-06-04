// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/AttackComponent.h"
#include "PlayerAttackComponent.generated.h"


UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ASCENSION_API UPlayerAttackComponent : public UAttackComponent
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties.
	UPlayerAttackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	/** Tracks how many attacks the user has chained. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combos")
	int ComboMeter;
	
public:
	/** Implementation for selecting attacks. */
	virtual void SelectAttack_Implementation(const FString& AttackType);

	/** Implementation for resetting attacks. */
	virtual void Reset_Implementation();
};
