// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "EnemyBoss.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API AEnemyBoss : public AEnemyCharacter
{
	GENERATED_BODY()

public:
	AEnemyBoss();

	virtual void DieEvent() override;
	virtual void BeginPlay() override;
	virtual void DestroyChar() override;
public:
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit|BossInfo")
	FName m_BossName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit|BossInfo")
	UTexture2D* m_BossCapture;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit|BossInfo")
	TArray<TSubclassOf<class AInterfaceProp_Inventory>> m_DropItemList;*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit|BossInfo")
	TArray<TSubclassOf<class ASkillBase>> m_SkillList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UCapsuleComponent* m_DamageRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|BossInfo")
	bool m_bDmgOn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UParticleSystemComponent* Trail;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UWidgetComponent* BossStatWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|BossInfo")
	float m_BossStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|BossInfo")
	float m_BossMaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|BossInfo")
	float m_StunTime;
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|BossInfo")
	EBossState m_eState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|BossInfo")
	EBossStage m_eStage;*/
public:
	UFUNCTION()
	virtual void OnDmgOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnDmgOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void NotifyActorOnClicked(FKey PressedButton = EKeys::LeftMouseButton) override;

	// 스킬별로.. 보스마다 다른 패턴 보여줘야 하니까... 블루프린트에서 커스터마이징 가능하도록 하자
	UFUNCTION(BlueprintImplementableEvent)
	void UsingSkill(int32 SkillIdx);

	UFUNCTION(BlueprintCallable)
	void m_BeginTrail();
	UFUNCTION(BlueprintCallable)
	void m_EndTrail();
};
