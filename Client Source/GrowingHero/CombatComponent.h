// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnumCollection.h"
#include "CombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GROWINGHERO_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	class AUnitBase* m_pTarget;
	ECharType m_eTargetType;
	FTimerHandle AttackTimer;
	FTimerHandle DeathTimer;
	AUnitBase* Owner;

	bool m_bIsComboOn;
	FName m_ComboSectionName;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf <class UDamageType> DamageTypeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UAnimMontage* CombatMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TArray<UAnimMontage*> m_arCombatMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	class UAnimInstance* AnimInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	int32 m_MaxAttackKind;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	bool m_bHitBackOn;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void TakeDamage(float Damage);

	UFUNCTION(BlueprintCallable)
	void TakeDamageEnd();

	UFUNCTION(BlueprintCallable)
	void ApplyDamage();

	/*UFUNCTION(BlueprintCallable)
	void ApplyDamageRadial();*/
	UFUNCTION(BlueprintCallable)
	void setInit(AUnitBase* pTarget, ECharType eTargetType);

	UFUNCTION(BlueprintCallable)
	void clearTarget();

	UFUNCTION(BlueprintCallable)
	void DieEnd();

	UFUNCTION(BlueprintCallable)
	void ReleaseCollision();

	UFUNCTION(BlueprintCallable)
	void Attack();
	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UFUNCTION(BlueprintCallable)
	void setComboOn(bool IsOn);

	UFUNCTION(BlueprintCallable)
	void setNextComboSectionName(FName NextSectionName);

	AUnitBase* getTarget();
	void setTargetClear();

	UFUNCTION(BlueprintCallable)
	void e_StunStart(float fStunTime);

	UFUNCTION(BlueprintCallable)
	void e_StunEnd();

private:
	void RotateToTarget(float DeltaTime);
	void Die();
	void comboActive();
	bool CanCombo();
};
