// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestMGR.h"
#include "MyCharacter.h"
#include "MyCharacterController.h"
#include "StatComponent.h"




void UQuestMGR::AddQuest(int32 QuestID)
{
	// DB에서 QuestID에 해당하는 퀘스트 정보를 TMap에 넣어준다. 
	FString RowName = FString::FromInt(QuestID);
	if ((QuestDB->FindRow<FQuestInfo>(FName(*RowName), RowName) != nullptr))
	{
		FQuestInfo QInfo = *(QuestDB->FindRow<FQuestInfo>(FName(*RowName), RowName));
		m_ManagedQuest.Add(QuestID , QInfo);
		if (ED_QuestAdded.IsBound() == true)
			ED_QuestAdded.Broadcast(QuestID);
	}
}

void UQuestMGR::RemoveQuest(int32 QuestID)
{
	// TMap에서 QuestID에 해당하는 것을 삭제한다. 
	m_ManagedQuest.Remove(QuestID);

	if (ED_QuestRemoved.IsBound() == true)
		ED_QuestRemoved.Broadcast(QuestID);
}

void UQuestMGR::CompleteQuest(int32 QuestID)
{
	m_CompletedQuest.Add(QuestID);
	// 보상해주기. 
	CompensateQuest(QuestID);
	
	// TMap에서 QuestID에 해당하는 것을 완료시킨다.
	if (ED_QuestCompleted.IsBound() == true) 
		ED_QuestCompleted.Broadcast(QuestID);

	if (m_ManagedQuest[QuestID].Next_QuestID > 0
		&& m_ManagedQuest.Find(m_ManagedQuest[QuestID].Next_QuestID) == nullptr)
		AddQuest(m_ManagedQuest[QuestID].Next_QuestID);

	RemoveQuest(QuestID);
}

void UQuestMGR::ProgressingQuest(int32 QuestID)
{
	// TMap에서 QuestID에 해당하는 것의 진행도를 1 올려준다.
	FQuestInfo* Q = m_ManagedQuest.Find(QuestID);
	if (Q != nullptr)
	{
		Q->Cur_Progress++;
		if (Q->Cur_Progress >= Q->Max_Progress)
			CompleteQuest(QuestID);
	}
}

bool UQuestMGR::FindCompletedQuest(int32 QuestID)
{
	if(m_CompletedQuest.Find(QuestID) != NULL)
		return true;
	return false;
}

void UQuestMGR::CompensateQuest(int32 QuestID)
{
	FQuestInfo* Q = m_ManagedQuest.Find(QuestID);
	int32 CompenEXP = Q->Compensation_Exp;
	
	if (!IsValid(m_pMyHero) || !IsValid(m_world))
	{
		return;
	}

	m_pMyHero->GainEXT(CompenEXP);

	for (auto m : Q->Compensation_Item)
	{
		// 아이템 필드로 스폰
		FTransform SpawnTransform{};
		SpawnTransform = m_pMyHero->GetMesh()->GetSocketTransform(FName("DropItemPoint"));
		for (int32 i = 0; i < m.Value; i++)
		{
			AInterfaceProp_Inventory* SpawnItem = m_world->SpawnActor<AInterfaceProp_Inventory>(m.Key.Get(), SpawnTransform);
			if (SpawnItem)
			{
				SpawnItem->pickup();
			}
		}
	}
}

