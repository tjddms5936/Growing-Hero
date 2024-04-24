// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnumCollection.h"
#include "HotKeyComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GROWINGHERO_API UHotKeyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHotKeyComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	int32 m_nMaxLength;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HotKey|SaveData")
	TArray<class UInterfaceWithHotKeySlot_Base*> m_arHotKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HotKey|SaveData")
	UInterfaceWithHotKeySlot_Base* m_DefaultSlot;

	void setHotKey(EKEY eKey, UInterfaceWithHotKeySlot_Base* pInterfaceSlot);

	void SwapHotKey(EKEY eDragSlotKey, EKEY eDropSlotKey);

	void ActivateHotKey(EKEY eActivateKey);
};
