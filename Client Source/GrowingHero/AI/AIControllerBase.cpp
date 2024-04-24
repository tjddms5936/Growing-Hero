// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "GrowingHero/UnitBase.h"

const FName AAIControllerBase::Key_Target(TEXT("Target"));
const FName AAIControllerBase::Key_SelfUnit(TEXT("SelfUnit"));

AAIControllerBase::AAIControllerBase()
{
	
}

void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (!IsValid(BTAsset) || !IsValid(BBAsset))
		return;

	RunAI();

	AUnitBase* TmpSelfUnit = Cast<AUnitBase>(GetPawn());
	Blackboard->SetValueAsObject(AAIControllerBase::Key_SelfUnit, TmpSelfUnit);
}

void AAIControllerBase::RunAI()
{
	UBlackboardComponent* l_BlackboardComponent = Blackboard.Get();
	if (UseBlackboard(BBAsset, l_BlackboardComponent))
	{
		RunBehaviorTree(BTAsset);
	}
}

void AAIControllerBase::StopAI()
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr == BehaviorTreeComponent) return;

	BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
}
