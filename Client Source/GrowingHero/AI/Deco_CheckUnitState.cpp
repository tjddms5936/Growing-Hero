// Fill out your copyright notice in the Description page of Project Settings.


#include "Deco_CheckUnitState.h"
#include "AIControllerBase.h"
#include "GrowingHero/UnitBase.h"
#include "BehaviorTree/BlackboardComponent.h"

UDeco_CheckUnitState::UDeco_CheckUnitState()
{
	NodeName = TEXT("CheckUnitState");
}

bool UDeco_CheckUnitState::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AUnitBase* SelfUnit = Cast<AUnitBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AAIControllerBase::Key_SelfUnit));
	if (SelfUnit->getUnitState() == m_eUnitState)
	{
		if (SelfUnit->getUnitState() == EUNIT_STATE::E_UnderAttack)
		{
		}
		return true;
	}

	return false;
}


