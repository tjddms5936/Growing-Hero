// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "GrowingHero/EnumCollection.h"
#include "Decorator_IsInAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UDecorator_IsInAttackRange : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UDecorator_IsInAttackRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom")
	ECharType m_eTargetType;
};
