// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InterfaceProp.h"
#include "InterfaceProp_Field.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API AInterfaceProp_Field : public AInterfaceProp
{
	GENERATED_BODY()
public:
	AInterfaceProp_Field() = default;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	virtual void ActivateSystem() override;

	virtual void NotifyActorOnClicked(FKey PressedButton = EKeys::LeftMouseButton) override;

};
