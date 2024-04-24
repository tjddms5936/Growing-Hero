// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitBase.h"
#include "StructureCollection.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API AEnemyCharacter : public AUnitBase
{
	GENERATED_BODY()
	
protected:
	// UPROPERTY(BlueprintReadOnly)
	class AMyCharacterController* m_pMyController;

	// UPROPERTY(BlueprintReadOnly)
	class AMyCharacter* m_pMyHero;
public:
	AEnemyCharacter();

	virtual void DieEvent() override;
	virtual void BeginPlay() override;
	virtual void DestroyChar() override; 
public:
	virtual void NotifyActorOnClicked(FKey PressedButton = EKeys::LeftMouseButton) override;

	AUnitBase* getTarget();
	void setTargetClear();

	void DropItem();
	void AddEXP();

	virtual void MediateAttackRange(float fAddRangeAmount) override;
	
	UFUNCTION()
	void OutlineOff();

	UFUNCTION(BlueprintCallable)
	void EnemyClickEvent();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit|EnemyInfo")
	FName m_EnemyName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit|EnemyInfo")
	UTexture2D* m_EnemyCapture;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit|EnemyInfo")
	TArray<TSubclassOf<class AInterfaceProp_Inventory>> m_DropItemList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* m_InterfaceRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit|EnemyInfo")
	TArray<int32> m_arRelevantQuestID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UQuestMGR* m_MainQuestMGR;

};


