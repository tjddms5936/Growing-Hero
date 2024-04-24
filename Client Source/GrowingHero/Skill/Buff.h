// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "GrowingHero/EnumCollection.h"
#include "Buff.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API ABuff : public ASkillBase
{
	GENERATED_BODY()
	
public:
	ABuff();

public:
	virtual void BeginPlay() override;
	virtual void Disappear() override;
	virtual void ActivateSkill() override;
	virtual void init(int32 nSkillLV, int32 nConsumeMP, int32 nSkillNum) override;

private:

	void DetectOwner_Specify();
	void DetectTarget_Specify();
	void DetectRange();

	void(ABuff::* m_arDetectFunc[(int)EDetectTargeType::E_MAX])();

	void findDetectTargetTagName();
	void LoadBuffEffect();
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Buff")
	int32 m_nSkillID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Buff")
	FName m_BuffName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* BuffRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Buff")
	TArray<TSubclassOf<class ABuffEffectBase>> m_arBuffEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Buff")
	TArray<ABuffEffectBase*> m_arBuffEffectRef;

	TArray<class AUnitBase*> m_arBuffTargets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Buff")
	TSubclassOf<ACharacter> DetectFilter;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Buff")
	EDetectTargeType m_eDetectType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill|Buff")
	ECharType m_eTargetType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Buff")
	float m_fDetectRadius;

	FName m_DetectTargetTagName;

public:
	void ReleaseBuffFromTarget(AUnitBase* pUnit);
};





