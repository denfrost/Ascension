// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Enemy.h"
#include "Damageable.h"
#include "Lockable.h"
#include "Goblin.generated.h"

/** Goblin
 *  A greatsword wielding enemy controlled by the AI.
 */
UCLASS()
class ASCENSION_API AGoblin : public AEnemy, public IDamageable, public ILockable
{
	GENERATED_BODY()
	
public:
	AGoblin();

	/** Current health of the character.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Parameters")
	float Health;

	/** Maximum health of the character.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Parameters")
	float MaxHealth;


	/** Returns the percentage of health remaining. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface Functions")
	void GetHealthPercent(float& HealthPercent);
	virtual void GetHealthPercent_Implementation(float& HealthPercent) override;

	/** Applies the effects of an attack to the entity. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface Functions")
	void ApplyHitEffect(const float Damage, const EHitEffect HitEffect);
	virtual void ApplyHitEffect_Implementation(const float Damage, const EHitEffect HitEffect) override;

	/** Shows the entity's health bar. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface Functions")
	void ShowHealthBar();
	virtual void ShowHealthBar_Implementation() override;

	/** Hides the entity's health bar. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface Functions")
	void HideHealthBar();
	virtual void HideHealthBar_Implementation() override;

	/** To check whether the entity is alive or not. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface Functions")
	bool IsDead();
	virtual bool IsDead_Implementation() override;

private:
	
};
