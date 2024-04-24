// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_StatFrame.h"
#include "UMG_StatWindow.h"

void UUMG_StatFrame::init()
{
	Super::init();
	StatWindow->init();
}

void UUMG_StatFrame::Setup()
{
	Super::Setup();
}

void UUMG_StatFrame::TearDown()
{
	Super::TearDown();
}
