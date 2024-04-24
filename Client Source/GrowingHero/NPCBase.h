// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StructureCollection.h"
#include "NPCBase.generated.h"

USTRUCT(BlueprintType)
struct FConversation
{
	GENERATED_USTRUCT_BODY()
public:
	FConversation()
	{
		IsLastConversation = false;
		BeforeConversationIndex = -1;
		arNextConversationIndex.Reserve(20); // 자리 예약만... 
		CurConversation = FText::FromString("Cur Conversation");
	}
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsLastConversation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BeforeConversationIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> arNextConversationIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText CurConversation;
};

UCLASS()
class GROWINGHERO_API ANPCBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPCBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	UTexture2D* m_NPC_Thumbnail;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* m_InterfaceRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	TArray<FConversation> m_FConvOrder;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Defaults")
	TArray<TSubclassOf<class AInterfaceProp_Inventory>> m_arOwnItemClasses;

	UPROPERTY(BlueprintReadWrite, Category = "Defaults")
	int32 m_nCurConvIndex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	int32 m_nDefaultConvIndex;

	class AMyCharacterController* m_pController;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Defaults")
	FName m_NPCName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UWidgetComponent* NPCNameWidget;

	// 관련된 퀘는... 배열로 들고 관련된 퀘 싹 다 진행시키면 될듯
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults")
	TArray<int32> m_arRelevantQuestID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	int32 m_OwnQuestID_Idx;

	// first : PreQuest ID. if -1, No PreQuest
	//second : NewQuest ID
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults")
	TArray<FPair_INT32> m_arOwnQuestID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UQuestMGR* m_MainQuestMGR;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults")
	class USoundBase* m_NPCSound;
public:
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ConversationStart();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ConversationEnd();

	UFUNCTION(BlueprintCallable)
	void RelevantQuestProgrssing();

	UFUNCTION(BlueprintCallable)
	void AddQuestFuncCall();
};
