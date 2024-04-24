// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIControllerBase.h"
#include "BossAIController.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API ABossAIController : public AAIControllerBase
{
	GENERATED_BODY()
	
public:
	ABossAIController();
	virtual void OnPossess(APawn* InPawn) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UAIPerceptionComponent* m_AIPerception;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class AEnemyBoss* m_ControlledBoss;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Customizing")
	TArray<FName> m_arSkillSectionName;

public:
	UFUNCTION(BlueprintCallable)
	FName GetRandomSectionName(int32 nMaxRandomSize);
};
