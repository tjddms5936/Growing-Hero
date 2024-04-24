// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UMG_Frame.h"
#include "UMG_EquipmentFrame.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UUMG_EquipmentFrame : public UUMG_Frame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UUMG_EquipmentWindow* EquipmentWindow;

	void UpdateEquipmentWindow();
	virtual void init() override;
	virtual void Setup() override;
	virtual void TearDown() override;
};
