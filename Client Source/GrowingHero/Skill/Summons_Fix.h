// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "GrowingHero/EnumCollection.h"
#include "Summons_Fix.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API ASummons_Fix : public ASkillBase
{
	GENERATED_BODY()
	
public:
	ASummons_Fix();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Disappear() override;
	virtual void ActivateSkill() override;
	virtual void init(int32 nSkillLV, int32 nConsumeMP, int32 nSkillNum) override;

private:
	TArray<AActor*> OverlappedActors;
	class AEnemyCharacter* m_pTarget;
	FTimerHandle DetectTimer;
	EUNIT_STATE m_eUnitState;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SKill|Customizing")
	class UAnimMontage* CombatMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UAnimInstance* AnimInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USphereComponent* AttackRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SKill|Customizing")
	TSubclassOf<AEnemyCharacter> EnemyFilter;

	// Re Attack Time. Less setting is meaning for more fast attack speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SKill|Customizing")
	float m_fAttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<class UDamageType> DamageTypeClass;
public:
	void AttackTarget();
	
	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UFUNCTION(BlueprintCallable)
	void SpawnEnd();

	UFUNCTION(BlueprintCallable)
	void ApplyDamage();

private:
	void DetectTarget();
	void RotateToTarget(float DeltaTime);

	UFUNCTION()
	virtual void AttackRangeOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
