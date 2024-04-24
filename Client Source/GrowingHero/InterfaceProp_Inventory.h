// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InterfaceProp.h"
#include "InterfaceProp_Inventory.generated.h"

/**
 * 
 */

UCLASS()
class GROWINGHERO_API AInterfaceProp_Inventory : public AInterfaceProp
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryProp|Info")
	FName m_ItemRowName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InventoryProp|Info")
	class USoundBase* PickUpSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InventoryProp|Info")
	TArray<int32> m_arRelevantQuestID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UQuestMGR* m_MainQuestMGR;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	virtual void ActivateSystem() override;

	virtual void NotifyActorOnClicked(FKey PressedButton = EKeys::LeftMouseButton) override;

	UFUNCTION(BlueprintCallable)
	void pickup();

	UFUNCTION(BlueprintImplementableEvent)
	void MouseCursorReset();
};
