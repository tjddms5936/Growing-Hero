// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrowingHero/EnumCollection.h"
#include "UISC_MGR.generated.h"

UCLASS()
class GROWINGHERO_API AUISC_MGR : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUISC_MGR();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void createEnergyControl(class UUserInterfaceSystemComponent*& pUISC);
	void createEquipSystem(UUserInterfaceSystemComponent*& pUISC);
	void createStatUpSystem(UUserInterfaceSystemComponent*& pUISC);

	void(AUISC_MGR::* m_arFunc[(int)EUISC_NAME2::E_MAX])(UUserInterfaceSystemComponent*& pUISC);

public:
	UFUNCTION(BlueprintCallable)
		void init();

	UFUNCTION(BlueprintCallable)
		UUserInterfaceSystemComponent* createUISC(EUISC_NAME2 eUISC_Name);

	UFUNCTION(BlueprintCallable)
		void releaseUISC(UUserInterfaceSystemComponent*& pUISC);
};
