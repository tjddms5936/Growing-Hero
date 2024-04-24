// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitBase.h"
#include "OtherPlayer.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API AOtherPlayer : public AUnitBase
{
	GENERATED_BODY()
	
public:
	AOtherPlayer();
	~AOtherPlayer();

public:
	virtual void BeginPlay();
	virtual void Tick(float DeltaTime) override;

public:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;


public:
	// ======================== 서버 동기화 =========================
	Protocol::PlayerState GetMoveState() { return m_PlayerInfo->state(); }
	virtual void SetPlayerInfo(const Protocol::PlayerInfo& Info) override;
	virtual void SetMoveState(Protocol::PlayerState StateInfo) override;
	virtual void SetDestInfo(const Protocol::PlayerInfo& Info, int32 DashIdx) override;
	Protocol::PlayerInfo* GetPlayerInfo() { return m_PlayerInfo; }

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DashStart(int32 nDashIdx);

	int32 m_DashIdx;

};
