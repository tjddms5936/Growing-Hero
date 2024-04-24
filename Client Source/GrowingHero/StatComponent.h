// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GROWINGHERO_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	class AUnitBase* m_pMyHero;

private:

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharStat")
	TMap<EStatName, float> m_mapCharStat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharStat")
	int32 m_nStatPoint;
public:	
	void init(AUnitBase* pMyChar);

	UFUNCTION(BlueprintCallable)
	int32 getStatPoint();
	
	UFUNCTION(BlueprintCallable)
	void AddStatPoint(int32 nPoint);

	UFUNCTION(BlueprintCallable)
	void StatUp(EStatName eStatName, float nStatupCount);

	UFUNCTION(BlueprintCallable)
	void UpdateStat();

	UFUNCTION(BlueprintCallable)
	void StatGameSaveSetting();

	UFUNCTION(BlueprintCallable)
	void StatGameLoad();
};
