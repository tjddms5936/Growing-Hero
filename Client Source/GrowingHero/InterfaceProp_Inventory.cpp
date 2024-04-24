// Fill out your copyright notice in the Description page of Project Settings.


#include "InterfaceProp_Inventory.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

#include "InventoryComponent.h"
#include "MyCharacter.h"
#include "MyCharacterController.h"
#include "MainGameInstance.h"
#include "QuestMGR.h"

void AInterfaceProp_Inventory::BeginPlay()
{
	Super::BeginPlay();
	m_pCollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &AInterfaceProp_Inventory::OnOverlapBegin);
	m_pCollisionVolume->OnComponentEndOverlap.AddDynamic(this, &AInterfaceProp_Inventory::OnOverlapEnd);
	m_MainQuestMGR = Cast<UMainGameInstance>(GetGameInstance())->m_QuestMGR;
}

void AInterfaceProp_Inventory::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (m_pMyController->getClickedActor() == this && !m_bIsInRange)
	{
		pickup();
	}
	m_bIsInRange = true;
}

void AInterfaceProp_Inventory::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	m_bIsInRange = false;
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AInterfaceProp_Inventory::ActivateSystem()
{
	Super::ActivateSystem();
}

void AInterfaceProp_Inventory::NotifyActorOnClicked(FKey PressedButton)
{
	Super::NotifyActorOnClicked(PressedButton);
	if (m_pMyController->getClickedActor() == this && m_bIsInRange)
	{
		pickup();
	}
}

void AInterfaceProp_Inventory::pickup()
{
	if (m_pMyController)
	{
		if (!(m_pMyController->InventorySystemComponent->AddItem(m_eItemType, m_ItemRowName)))
			GEngine->AddOnScreenDebugMessage(1, 2, FColor::Black, FString::Printf(TEXT("Inventory Full")));
		else
		{
			MouseCursorReset();
			m_pMyController->InventoryFrameRefresh(m_eItemType); // UI 업데이트
			UGameplayStatics::SpawnSound2D(GetWorld(), PickUpSound);
			m_pMyController->m_pClickedProp = nullptr;
			AInterfaceProp::DestroyActor();

			for (auto ID : m_arRelevantQuestID)
			{
				if (ID > 0)
					m_MainQuestMGR->ProgressingQuest(ID);
			}
		}
	}
}


