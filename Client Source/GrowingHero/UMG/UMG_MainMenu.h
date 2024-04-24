// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG_MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UUMG_MainMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	UUMG_MainMenu(const FObjectInitializer& ObjectInitializer);
	virtual bool Initialize() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UWidgetAnimation* ClickAnimation;

private:
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* m_LogInMenu;
	
	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* LoadMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* GuideMenu;

	// ======================== ¼öÁ¤ Áß ========================
	UPROPERTY(meta = (BindWidget))
	class UButton* GameStartBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* OpenLogInMenuBtn;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* m_NameTextBlock;
	// ==========================================================

	UPROPERTY(meta = (BindWidget))
	class UButton* OpenGameLoadMenuBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* OpenGameGuidMenueBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* GameLoadMenuBackBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* GameGuideMenuBackBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* LoadGameBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitBtn;
	
	
public:
	UFUNCTION(BlueprintCallable)
	void GameLogInMenuOpen();

	UFUNCTION(BlueprintCallable)
	void GameLoadMenuOpen();

	UFUNCTION(BlueprintCallable)
	void GameGuideMenuOpen();

	UFUNCTION(BlueprintCallable)
	void MenuCancel();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void GameStart();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void LoadGame();

	UFUNCTION()
	void ExitGame();

	UFUNCTION()
	void OnLogInTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION(BlueprintCallable)
	void SendLogIn(const FText& Text);

	UFUNCTION(BlueprintCallable)
	bool IsNameEmpty();

	UFUNCTION(BlueprintCallable)
	const FText GetPlayerName();
private:
	bool m_bIsLastTextEmpty;
};
