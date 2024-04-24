// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GrowingHero/AI/AIControllerBase.h"
#include "SummonAIController.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API ASummonAIController : public AAIControllerBase
{
	GENERATED_BODY()
	
public:
	ASummonAIController();

	virtual void OnPossess(APawn* InPawn) override;
	static const FName Key_Owner;

};
