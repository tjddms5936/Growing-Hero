// Fill out your copyright notice in the Description page of Project Settings.


#include "Decorator_IsInAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "AIControllerBase.h"
#include "GrowingHero/UnitBase.h"

UDecorator_IsInAttackRange::UDecorator_IsInAttackRange()
{
	NodeName = TEXT("Check Is In Attack Range");
}

bool UDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = false;

	AUnitBase* SelfUnit = Cast<AUnitBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AAIControllerBase::Key_SelfUnit));
	AUnitBase* Target = Cast<AUnitBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AAIControllerBase::Key_Target));

	if (SelfUnit->getUnitState() == EUNIT_STATE::E_Dead)
	{
		return bResult;
	}

	if (Target->GetDistanceTo(SelfUnit) <= SelfUnit->getUnitStat()->AttackRange)
	{
		bResult = true;
	}
	return bResult;
}

