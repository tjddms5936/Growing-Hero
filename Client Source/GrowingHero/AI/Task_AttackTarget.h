// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GrowingHero/EnumCollection.h"
#include "Task_AttackTarget.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UTask_AttackTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UTask_AttackTarget();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom")
	ECharType m_eTargetType;
};
