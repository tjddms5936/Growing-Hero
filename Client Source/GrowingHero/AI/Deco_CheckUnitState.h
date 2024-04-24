// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "GrowingHero/EnumCollection.h"
#include "Deco_CheckUnitState.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UDeco_CheckUnitState : public UBTDecorator
{
	GENERATED_BODY()
public:
	UDeco_CheckUnitState();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom")
	EUNIT_STATE m_eUnitState;
};


