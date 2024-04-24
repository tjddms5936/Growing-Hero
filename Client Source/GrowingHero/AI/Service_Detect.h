// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "GrowingHero/EnumCollection.h"
#include "Service_Detect.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UService_Detect : public UBTService
{
	GENERATED_BODY()
public:
	UService_Detect();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom")
	ECharType m_eTargetType;
};
