// Fill out your copyright notice in the Description page of Project Settings.


#include "Tast_SetUnitState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIControllerBase.h"
#include "GrowingHero/UnitBase.h"

UTast_SetUnitState::UTast_SetUnitState()
{
    NodeName = TEXT("SetUnitState");
}

EBTNodeResult::Type UTast_SetUnitState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AUnitBase* SelfUnit = Cast<AUnitBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AAIControllerBase::Key_SelfUnit));

    if (SelfUnit->getUnitState() == EUNIT_STATE::E_Dead)
    {
        return EBTNodeResult::Failed;
    }
    SelfUnit->setUnitState(m_eUnitState);
    return EBTNodeResult::Succeeded;
}
