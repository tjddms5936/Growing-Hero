// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GrowingHero/EnumCollection.h"
#include "Interface_Window_Slot.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterface_Window_Slot : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GROWINGHERO_API IInterface_Window_Slot
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SwapSlot(EItemType eItemType,int32 nDragSlotIndex, int32 nDropSlotIndex) abstract;
	virtual void UpdateSlot(int32 nidx) abstract;
};


