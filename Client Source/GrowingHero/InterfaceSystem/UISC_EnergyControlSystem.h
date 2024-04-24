// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserInterfaceSystemComponent.h"
#include "UISC_EnergyControlSystem.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UUISC_EnergyControlSystem : public UUserInterfaceSystemComponent
{
	GENERATED_BODY()
	

private:
	void AddHP();
	void setAddHP(float fAddHP);

private:
	float m_fAddHP;

public:
	virtual void ActiveSystem() override;

	UFUNCTION(BlueprintCallable)
	void setInitSystem(float fInitValue);
};
