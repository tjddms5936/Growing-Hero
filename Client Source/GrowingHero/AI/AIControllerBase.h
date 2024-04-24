// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()

public:
	AAIControllerBase();

	virtual void OnPossess(APawn* InPawn) override;
	static const FName Key_Target;
	static const FName Key_SelfUnit;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Default")
	class UBehaviorTree* BTAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Default")
	class UBlackboardData* BBAsset;

public:
	void RunAI();
	void StopAI();
};
