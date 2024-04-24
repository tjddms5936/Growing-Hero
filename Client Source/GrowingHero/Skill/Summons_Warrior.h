// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "Summons_Warrior.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API ASummons_Warrior : public ASkillBase
{
	GENERATED_BODY()
	
public:
	ASummons_Warrior();
	virtual void BeginPlay() override;
	virtual void Disappear() override;
	virtual void ActivateSkill() override;
	virtual void init(int32 nSkillLV, int32 nConsumeMP, int32 nSkillNum) override;
private:
	int32 m_nMaxLength;

private:
	void SpawningWarrior();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Customizing")
	TSubclassOf<class ASummoned_Warrior> SpawnWarrior;

	ASummoned_Warrior* m_pSpawnedWarrior;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill|Customizing")
	TArray<ASummoned_Warrior*> m_ppSpawnedWarrior;

};
