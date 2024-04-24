// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserInterfaceSystemComponent.h"
#include "UISC_StatUp.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UUISC_StatUp : public UUserInterfaceSystemComponent
{
	GENERATED_BODY()
	
public:
	UUISC_StatUp();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 m_nMaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 m_nMaxMP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 m_nCurEXP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 m_nATK;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 m_nDEF;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 m_nSTR;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 m_nDEX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 m_nINT;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 m_nLUK;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float m_fAttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float m_fAttackSpeed;
public:
	virtual void ActiveSystem() override;

	UFUNCTION(BlueprintCallable)
	void setInitSystem(int32 nMaxHP = 0, int32 nMaxMP = 0, int32 nCurEXP = 0, int32 nATK = 0, int32 nDEF = 0, int32 nSTR = 0, 
		int32 nDEX = 0, int32 nINT = 0, int32 nLUK = 0, float fAttackRange = 0.f, float fAttackSpeed = 0.f);
};
