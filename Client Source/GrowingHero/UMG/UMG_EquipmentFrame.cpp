// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_EquipmentFrame.h"
#include "UMG_EquipmentWindow.h"

void UUMG_EquipmentFrame::UpdateEquipmentWindow()
{
	EquipmentWindow->UpdateEquipmentSlot();
}

void UUMG_EquipmentFrame::init()
{
	Super::init();
	EquipmentWindow->init();
}

void UUMG_EquipmentFrame::Setup()
{
	Super::Setup();
}


void UUMG_EquipmentFrame::TearDown()
{
	Super::TearDown();
}
