// Fill out your copyright notice in the Description page of Project Settings.


#include "Summons_Warrior.h"
#include "Summoned_Warrior.h"

ASummons_Warrior::ASummons_Warrior() :
	m_nMaxLength{},
	m_ppSpawnedWarrior{}
{
	m_bIsDuplicate = false;
}

void ASummons_Warrior::BeginPlay()
{
	Super::BeginPlay();
}

void ASummons_Warrior::Disappear()
{
	DestroyEvent();
	for (int i = 0; i < m_nMaxLength; i++)
	{
		if(!m_ppSpawnedWarrior[i]->IsValidLowLevel()) 
			return;

		m_ppSpawnedWarrior[i]->releaseTargetingMe();
		m_ppSpawnedWarrior[i]->Disappear();

		m_ppSpawnedWarrior[i] = nullptr;
	}
	Super::Disappear();
}

void ASummons_Warrior::ActivateSkill()
{
	m_nMaxLength = m_nSkillLv;
	m_ppSpawnedWarrior.Reserve(m_nMaxLength);
	m_ppSpawnedWarrior.Init(nullptr, m_nMaxLength);
	m_bIsActivated = true;
	SpanwSkillNiagara_Sound();

	Super::ActivateSkill();
	SpawningWarrior();
}

void ASummons_Warrior::init(int32 nSkillLV, int32 nConsumeMP, int32 nSkillNum)
{
	Super::init(nSkillLV, nConsumeMP, nSkillNum);
	m_fHoldingTime *= nSkillLV;
	FTimerHandle DestroyTimer{};
	GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &ASummons_Warrior::CallRelease, m_fHoldingTime, false, (-1.0F));
	ActivateSkill();
}

void ASummons_Warrior::SpawningWarrior()
{
	if (SpawnWarrior.Get())
	{
		UWorld* world = GetWorld();
		FRotator rotator{};
		FVector location{};
		rotator = m_pSkillOwner->GetActorRotation();
		location = m_pSkillOwner->GetMesh()->GetSocketLocation("SpawnSkillPoint");

		for (int i = 0; i < m_nMaxLength; i++)
		{
			FActorSpawnParameters spawnParams{};
			location.X += FMath::RandRange(-100.f, 100.f);
			location.Y += FMath::RandRange(-100.f, 100.f);
			while (m_ppSpawnedWarrior[i] == nullptr)
			{
				m_ppSpawnedWarrior[i] = world->SpawnActor<ASummoned_Warrior>(SpawnWarrior.Get(), location, rotator, spawnParams);
				location.X += FMath::RandRange(-100.f, 100.f);
				location.Y += FMath::RandRange(-100.f, 100.f);
			}
			m_ppSpawnedWarrior[i]->initSummonedStat(m_nSkillLv);
		}
	}
}


