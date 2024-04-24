// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GROWINGHERO_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipmentComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	

private:
	int32 m_nMaxLength;
	class AMyCharacterController* m_pMyController;
	class AMyCharacter* m_pMyHero;
	FInventoryEquipment m_FDefaultEquipItemInfo;
	class UUnitAnimInstance* m_pMyAnimInstance;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EquipmentComponent|Info")
	TArray<FInventoryEquipment> m_arStrEquipItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EquipmentComponent|Info")
	TArray<class AEquipItem*> m_arEquipItem;

	void init();
	void EnterEquipment(enum class EEquipType eType, FInventoryEquipment FItem, AEquipItem* pEquipItem);
	void ReleaseEquipment(enum class EEquipType eType, bool IsDrop);

	UFUNCTION(BlueprintCallable)
	void LoadEquipment();
};
