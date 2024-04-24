// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GrowingHero/UnitBase.h"
#include "Summoned_Warrior.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API ASummoned_Warrior : public AUnitBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	ASummoned_Warrior();

public:
	void initSummonedStat(int nSkillLv);
	void releaseTargetingMe();
private:
	TArray<class AEnemyCharacter*> m_sTargetingMe;

	virtual void MediateAttackRange(float fAddRangeAmount) override;
};
