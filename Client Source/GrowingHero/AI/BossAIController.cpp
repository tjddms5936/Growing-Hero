// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAIController.h"
#include "GrowingHero/EnemyBoss.h"
#include "Perception/AIPerceptionComponent.h"

ABossAIController::ABossAIController() :
	m_arSkillSectionName{}
{
	m_AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	
}

void ABossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	m_ControlledBoss = Cast<AEnemyBoss>(InPawn);
}

FName ABossAIController::GetRandomSectionName(int32 nMaxRandomSize)
{
	if (m_arSkillSectionName.Num() < nMaxRandomSize)
		return FName("Skill1");

	int32 Section = FMath::RandRange(0, nMaxRandomSize - 1);
	return m_arSkillSectionName[Section];
}
