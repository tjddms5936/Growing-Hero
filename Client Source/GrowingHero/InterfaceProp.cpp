// Fill out your copyright notice in the Description page of Project Settings.


#include "InterfaceProp.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacterController.h"
#include "MyCharacter.h"
#include "InterfaceSystem/UISC_MGR.h"
#include "InterfaceSystem/UserInterfaceSystemComponent.h"

// Sets default values
AInterfaceProp::AInterfaceProp() :
	m_bIsInRange{},
	m_arSystemComponent{},
	m_eItemType{}
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = m_pMesh;

	m_pCollisionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionVolume"));
	m_pCollisionVolume->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AInterfaceProp::BeginPlay()
{
	Super::BeginPlay();

	m_pUISC_MGR = NewObject<AUISC_MGR>(this);
	m_pMyController = Cast<AMyCharacterController>(GetWorld()->GetFirstPlayerController());

}

AUISC_MGR* AInterfaceProp::getUISC_MGR()
{
	if(m_pUISC_MGR != nullptr)
		return m_pUISC_MGR;

	m_pUISC_MGR = NewObject<AUISC_MGR>(this);
	return m_pUISC_MGR;
}

void AInterfaceProp::ActivateSystem()
{
	if (m_pInterfacedUnit == nullptr)
	{
		return;
	}

	UGameplayStatics::SpawnSound2D(GetWorld(), ActivateSystemSound);
	for (auto v : m_arSystemComponent)
	{
		if (v)
		{
			v->setInterfacedUnit(m_pInterfacedUnit);
			v->ActiveSystem();
		}
	}
}

// Called every frame
void AInterfaceProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInterfaceProp::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag("Hero") || !OtherActor->ActorHasTag("ExpGratee"))
		return;

	m_pInterfacedUnit = Cast<AMyCharacter>(OtherActor);
}

void AInterfaceProp::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor->ActorHasTag("Hero") || !OtherActor->ActorHasTag("ExpGratee"))
		return;

	for (auto v : m_arSystemComponent)
	{
		if (v)
			v->setInterfacedUnit(nullptr);
	}

	m_pInterfacedUnit = nullptr;
	if(m_pMyController->getClickedActor() == this)
		m_pMyController->m_pClickedProp = nullptr;
}

void AInterfaceProp::NotifyActorOnClicked(FKey PressedButton)
{
	TArray<AActor*> OverlappingActors{};
	m_pCollisionVolume->GetOverlappingActors(OverlappingActors);
	for (auto v : OverlappingActors)
	{
		if (!v->ActorHasTag("Hero") || !v->ActorHasTag("ExpGratee"))
			continue;

		AMyCharacter* MyChar = Cast<AMyCharacter>(v);
		if (MyChar)
		{
			m_pMyController->setClickedActor(this);
			m_pInterfacedUnit = MyChar;
			break;
		}
	}
}

AMyCharacter* AInterfaceProp::getInterfacedUnit()
{
	return m_pInterfacedUnit;
}

void AInterfaceProp::setInterfacedUnit(AMyCharacter* pInterfacedUnit)
{
	m_pInterfacedUnit = pInterfacedUnit;
}

void AInterfaceProp::DestroyActor()
{
	for (auto v : m_arSystemComponent)
	{
		if (v)
		{
			m_pUISC_MGR->releaseUISC(v);
			v = nullptr;
		}
	}

	if (m_pMyController && m_pMyController->getClickedActor() == this)
	{
		m_pMyController->m_pClickedProp = nullptr;
	}
	Destroy();
}



