// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UserInterfaceSystemComponent.generated.h"


UCLASS(Abstract,  ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GROWINGHERO_API UUserInterfaceSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUserInterfaceSystemComponent();
	~UUserInterfaceSystemComponent() = default;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	class AUnitBase* m_pInterfacedUnit;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void ActiveSystem();

	void setInterfacedUnit(AUnitBase* pInterfacedUnit);
};
