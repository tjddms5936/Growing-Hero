// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GrowingHero/AI/AIControllerBase.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API AEnemyAIController : public AAIControllerBase
{
	GENERATED_BODY()
	
public:
	AEnemyAIController();

	virtual void OnPossess(APawn* InPawn) override;
	static const FName Key_HomePos;
	static const FName Key_PatrolPos;
};
