// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EquipItem.generated.h"

UCLASS()
class GROWINGHERO_API AEquipItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEquipItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	/** Base Mesh Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* CollisionVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UParticleSystemComponent* Trail;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USphereComponent* BloodRange;
public:
	UFUNCTION(BlueprintCallable)
	void m_BeginTrail();
	UFUNCTION(BlueprintCallable)
	void m_EndTrail();
};
