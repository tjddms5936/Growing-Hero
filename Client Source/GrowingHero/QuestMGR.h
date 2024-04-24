// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StructureCollection.h"
#include "QuestMGR.generated.h"

DECLARE_DELEGATE(FDele_Single);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FQuestDele_Dynamic);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestDele_D_One, const int32&, QuestID);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GROWINGHERO_API UQuestMGR : public UActorComponent
{
	GENERATED_BODY()

public:
	// Key : QuestID
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest|Info")
	TMap<int32, FQuestInfo> m_ManagedQuest;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quest|Info")
	class UDataTable* QuestDB;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Quest|Info")
	TSet<int32> m_CompletedQuest;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	FQuestDele_D_One ED_QuestCompleted; // EnemyCharacter에서 호출 바인딩 다함. 퀘스트창에서는 바인딩
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	FQuestDele_D_One ED_QuestAdded; // 퀘스트창에서는 바인딩
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	FQuestDele_D_One ED_QuestRemoved; // 퀘스트창에서는 바인딩


	UPROPERTY(BlueprintReadOnly)
	class AMyCharacter* m_pMyHero;
	UPROPERTY(BlueprintReadOnly)
	class AMyCharacterController* m_pMyController;

	UWorld* m_world;
public:
	UFUNCTION(BlueprintCallable)
	void AddQuest(int32 QuestID);

	UFUNCTION(BlueprintCallable)
	void RemoveQuest(int32 QuestID);

	UFUNCTION(BlueprintCallable)
	void CompleteQuest(int32 QuestID);

	UFUNCTION(BlueprintCallable)
	void ProgressingQuest(int32 QuestID);

	UFUNCTION(BlueprintCallable)
	bool FindCompletedQuest(int32 QuestID);

	UFUNCTION(BlueprintCallable)
	void CompensateQuest(int32 QuestID);
};
