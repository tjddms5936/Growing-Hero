// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "Projectile.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API AProjectile : public ASkillBase
{
	GENERATED_BODY()
	
public:
	AProjectile();
	virtual void BeginPlay() override;
	virtual void Disappear() override;
	virtual void ActivateSkill() override;
	virtual void init(int32 nSkillLV, int32 nConsumeMP, int32 nSkillNum) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* CollisionVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCapsuleComponent* DetectTargetRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Customizing")
	TSubclassOf<class UDamageType> DamageTypeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Customizing")
	TSubclassOf<class AEnemyCharacter> EnemyFilter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Customizing")
	float m_fProjectileSpeed;
private:
	AEnemyCharacter* m_pTarget;
	TArray<AEnemyCharacter*> m_arTargetInRange;
	bool IsDestroy;
	bool IsInitOK;
	
public:
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void ApplyDmg();

	UFUNCTION(BlueprintImplementableEvent)
	void setHomingTarget(AEnemyCharacter* pTarget);

	UFUNCTION(BlueprintCallable)
	AEnemyCharacter* getTarget();
private:
	void DetectTarget();
	void FindTargetInRange();
	void FindClosestTarget();
	void LookAtTarget();
	void setProjectileSpeed();
	void BoomEvent();
	void ActivateFailed();
};
