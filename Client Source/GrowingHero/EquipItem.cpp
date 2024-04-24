// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AEquipItem::AEquipItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionVolume"));
	RootComponent = CollisionVolume;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	Trail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail"));
	Trail->SetupAttachment(Mesh);

	BloodRange = CreateDefaultSubobject<USphereComponent>(TEXT("BloodRange"));
	BloodRange->SetupAttachment(Mesh);
	
}

// Called when the game starts or when spawned
void AEquipItem::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEquipItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEquipItem::m_BeginTrail()
{
	Trail->BeginTrails("TB", "TE", ETrailWidthMode::ETrailWidthMode_FromFirst, 2.0f);
}

void AEquipItem::m_EndTrail()
{
	Trail->EndTrails();
}
