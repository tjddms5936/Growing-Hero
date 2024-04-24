// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StructureCollection.h"
#include "SkillComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GROWINGHERO_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	int32 m_nMaxLength;
	class AUnitBase* m_pOwnerUnit;
	TArray<bool> m_bCanSkillActive;
	TArray<bool> m_bCanCoolDown;
public:
	// Save Data
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SKill|SaveData")
	int32 m_nSkillPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SKill|SaveData")
	TArray<FSkillData> m_arSkill;
public:
	TArray<FName> m_arSkillMontageName;
	TMap<ESkillNum, ASkillBase*> m_mapCantDuplicatedSkill;
	UPROPERTY(BlueprintReadOnly)
	class AMyCharacterController* m_pMyController;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Info")
	class UDataTable* SkillDB;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SKill|Info")
	class UAnimMontage* SkillMontage;
private:
	void initSkill();
	bool ConsumeMP(int32 nNeedMP);

	ASkillBase* SpawnSkill(ESkillNum eSkillNum);
	void resetSkillCoolTime(int32 nSkillNum);
public:
	void init();
	void releaseSkill(ASkillBase* pReleaseSkill);
	void SKillLevelUp(ESkillNum eSkillNum);

	UFUNCTION()
	void SkillAnimPlay(int32 nSkillNum);

	UFUNCTION(BlueprintCallable)
	int32 getSkillPoint();

	void UsingSkill(ESkillNum eSkillNum);

	UFUNCTION(BlueprintCallable)
	void AddSkillPoint(int32 nPoint);

	UFUNCTION(BlueprintCallable)
	void SkillAnimPlayEnd();

	UFUNCTION(BlueprintCallable)
	bool IsSkillCoolTimeOK(ESkillNum eSkillNum);

	UFUNCTION(BlueprintCallable)
	bool IsSkillCanActivat(ESkillNum eSkillNum);
};


