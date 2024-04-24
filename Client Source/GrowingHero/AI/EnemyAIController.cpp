// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "UObject/ConstructorHelpers.h"

const FName AEnemyAIController::Key_HomePos(TEXT("HomePos"));
const FName AEnemyAIController::Key_PatrolPos(TEXT("PatrolPos"));

AEnemyAIController::AEnemyAIController()
{
	
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}
