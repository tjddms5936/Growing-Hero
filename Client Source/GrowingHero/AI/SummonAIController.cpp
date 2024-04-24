// Fill out your copyright notice in the Description page of Project Settings.


#include "SummonAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"

#include "GrowingHero/MyCharacterController.h"
#include "GrowingHero/MyCharacter.h"

const FName ASummonAIController::Key_Owner(TEXT("Owner"));

ASummonAIController::ASummonAIController()
{
	
}

void ASummonAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AMyCharacterController* MasterController = Cast<AMyCharacterController>(GetWorld()->GetFirstPlayerController());
	if (MasterController == nullptr)
		return;

	AMyCharacter* Master = Cast<AMyCharacter>(MasterController->GetPawn());
	if (Master == nullptr)
		return;

	Blackboard->SetValueAsObject(ASummonAIController::Key_Owner, Master);
}