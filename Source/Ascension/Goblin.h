// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Enemy.h"
#include "GameFramework/Character.h"
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

	/** Event called to play footstep sounds. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void FootstepSound();

	/** Enables entity movement. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void EnableMovement();

	/** Disables entity movement. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void DisableMovement();

protected:
	UFUNCTION(BlueprintCallable, Category = "Helper")
	bool CheckDead();

	/** Displays the visuals required when the character is attacked. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void ShowHitVisuals();

	/** Applies the effects of an attack onto the character. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void ApplyAttackEffects(float Damage);

	/** Decrements the entity's health by the specified amount. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void DecrementHealth(float Damage);

	/** Performs the necessary steps to kill the actor. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void KillActor();

private:
	
};
