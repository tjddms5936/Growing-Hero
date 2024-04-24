// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CameraTransparencyComponent.generated.h"


USTRUCT(BlueprintType)
struct FCameraOccludedActor
{
	GENERATED_USTRUCT_BODY()
public:
	FCameraOccludedActor()
	{
		Actor = nullptr;
		StaticMesh = nullptr;
		Materials.Reserve(100);
		IsOccluded = false;
	}
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		const AActor* Actor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<UMaterialInterface*> Materials;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool IsOccluded;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GROWINGHERO_API UCameraTransparencyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCameraTransparencyComponent();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/** How much of the Pawn capsule Radius and Height
	 * should be used for the Line Trace before considering an Actor occluded?
	 * Values too low may make the camera clip through walls.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Occlusion|Occlusion",
		meta = (ClampMin = "0.1", ClampMax = "10.0"))
		float CapsulePercentageForTrace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Occlusion|Materials")
		UMaterialInterface* FadeMaterial;

	UPROPERTY(BlueprintReadWrite, Category = "Camera Occlusion|Components")
		class USpringArmComponent* ActiveSpringArm;

	UPROPERTY(BlueprintReadWrite, Category = "Camera Occlusion|Components")
		class UCameraComponent* ActiveCamera;

	UPROPERTY(BlueprintReadWrite, Category = "Camera Occlusion|Components")
		class UCapsuleComponent* ActiveCapsuleComponent;

	UPROPERTY(BlueprintReadWrite, Category = "Camera Occlusion|Components")
		class APawn* ActivePawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Occlusion")
		bool IsOcclusionEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Occlusion|Occlusion")
		bool DebugLineTraces;

private:
	TMap<const AActor*, FCameraOccludedActor> OccludedActors;

	bool HideOccludedActor(const AActor* Actor);
	bool OnHideOccludedActor(const FCameraOccludedActor& OccludedActor) const;
	void ShowOccludedActor(FCameraOccludedActor& OccludedActor);
	bool OnShowOccludedActor(const FCameraOccludedActor& OccludedActor) const;
	void ForceShowOccludedActors();

	__forceinline bool ShouldCheckCameraOcclusion() const
	{
		return IsOcclusionEnabled && FadeMaterial && ActiveCamera && ActiveCapsuleComponent;
	}

public:
	void init(USpringArmComponent* SpringArm, UCameraComponent* Camera, UCapsuleComponent* Capsule, APawn* Pawn);
	UFUNCTION(BlueprintCallable)
		void SyncOccludedActors();
};
