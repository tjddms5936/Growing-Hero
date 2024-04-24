// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/WidgetAnimation.h"
#include "Components/EditableTextBox.h"
#include "MainGameInstance.h"

UUMG_MainMenu::UUMG_MainMenu(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{

}

bool UUMG_MainMenu::Initialize()
{
	bool Success = Super::Initialize(); // 혹시 Initialize가 false를 반환 할 수 있으므로
	if (!Success) return false;
	
	if (!ensure(ExitBtn != nullptr))
		return false;
	ExitBtn->OnClicked.AddDynamic(this, &UUMG_MainMenu::ExitGame);
	m_NameTextBlock->OnTextCommitted.AddDynamic(this, &UUMG_MainMenu::OnLogInTextCommitted);

	return true;
}


void UUMG_MainMenu::GameLogInMenuOpen()
{
	if (!ensure(MenuSwitcher != nullptr))
		return;
	if (!ensure(m_LogInMenu != nullptr))
		return;

	MenuSwitcher->SetActiveWidget(m_LogInMenu);
}

void UUMG_MainMenu::GameLoadMenuOpen()
{
	if (!ensure(MenuSwitcher != nullptr)) 
		return;
	if (!ensure(LoadMenu != nullptr)) 
		return;

	MenuSwitcher->SetActiveWidget(LoadMenu);
}

void UUMG_MainMenu::GameGuideMenuOpen()
{
	if (!ensure(MenuSwitcher != nullptr))
		return;
	if (!ensure(GuideMenu != nullptr))
		return;

	MenuSwitcher->SetActiveWidget(GuideMenu);
}

void UUMG_MainMenu::MenuCancel()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(MainMenu != nullptr)) return;

	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UUMG_MainMenu::ExitGame()
{
	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

void UUMG_MainMenu::OnLogInTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	switch (CommitMethod)
	{
	case ETextCommit::OnEnter:
	{
		if (!Text.IsEmpty())
		{
			SendLogIn(Text);
			m_NameTextBlock->SetText(FText());
			m_bIsLastTextEmpty = false;
			GameStart();
		}
		else
		{
			m_bIsLastTextEmpty = true;
		}
		break;
	}
	default:
		break;
	}
}

void UUMG_MainMenu::SendLogIn(const FText& Text)
{
	UMainGameInstance* GameIns = Cast<UMainGameInstance>(GetGameInstance());
	if (GameIns)
	{
		GameIns->ConnectServer(Text);
	}
}

bool UUMG_MainMenu::IsNameEmpty()
{
	return m_NameTextBlock->GetText().IsEmpty();
}

const FText UUMG_MainMenu::GetPlayerName()
{
	// TODO: 여기에 return 문을 삽입합니다.
	return m_NameTextBlock->GetText();
}
