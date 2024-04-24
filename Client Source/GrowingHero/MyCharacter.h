// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitBase.h"
#include "MyCharacter.generated.h"

/**/

UCLASS()
class GROWINGHERO_API AMyCharacter : public AUnitBase
{
	GENERATED_BODY()

public:
    AMyCharacter();
    
private:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void initStat(float MaxHP) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void DestroyChar() override;
    virtual void DieEvent() override;
    virtual bool IsAdventureMode() override;

private:
    class AMyCharacterController* m_pMyController;
    class AEnemyCharacter* m_pEnemy;
    TArray<AEnemyCharacter*> OverlappingEnemies;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class USpringArmComponent* CameraSpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class USphereComponent* CombatRange;


    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit|Effect")
    class UNiagaraSystem* LevelUpEffect;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit|Sound")
    class USoundBase* LevelUpSound;

public:
    UFUNCTION(BlueprintCallable)
    void AttackEnemy(class AEnemyCharacter* Enemy);
    void GainEXT(int32 nEXP);
    void LevelUp();

    virtual void MediateAttackRange(float fAddRangeAmount) override;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void GameOverUMG();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void HPWarningOff();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void ManageDash(int32 Idx);
public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    float BaseTurnRate;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    float BaseLookUpRate;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool m_bIsAdventureMode;

    int32 m_nOpenedUICount;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Unit|Sync")
    int32 m_DashIdx;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Unit|Sync")
    float m_DesiredDashYaw;
};
