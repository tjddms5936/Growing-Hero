// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GrowingHero/EnumCollection.h"
#include "Tast_SetUnitState.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UTast_SetUnitState : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UTast_SetUnitState();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom")
	EUNIT_STATE m_eUnitState;
};


