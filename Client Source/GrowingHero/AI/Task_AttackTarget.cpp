// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_AttackTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIControllerBase.h"
#include "GrowingHero/UnitBase.h"
#include "GrowingHero/CombatComponent.h"

UTask_AttackTarget::UTask_AttackTarget()
{
	NodeName = TEXT("Attack Target");
}

EBTNodeResult::Type UTask_AttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AUnitBase* SelfUnit = Cast<AUnitBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AAIControllerBase::Key_SelfUnit));
	AUnitBase* Target = Cast<AUnitBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AAIControllerBase::Key_Target));

	if (!Target)
		return EBTNodeResult::Failed;

	if (SelfUnit->getUnitState() == EUNIT_STATE::E_Dead)
	{
		return EBTNodeResult::Succeeded;
	}

	SelfUnit->CombatComponent->setInit(Target, Target->getUnitType());
	SelfUnit->CombatComponent->Attack();
	return EBTNodeResult::Succeeded;
}



