// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_FindPatrolPos.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UTask_FindPatrolPos : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UTask_FindPatrolPos();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
