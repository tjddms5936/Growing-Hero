// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterfaceSystemComponent.h"

// Sets default values for this component's properties
UUserInterfaceSystemComponent::UUserInterfaceSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UUserInterfaceSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UUserInterfaceSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UUserInterfaceSystemComponent::ActiveSystem()
{

}

void UUserInterfaceSystemComponent::setInterfacedUnit(AUnitBase* pInterfacedUnit)
{
	m_pInterfacedUnit = pInterfacedUnit;
}

