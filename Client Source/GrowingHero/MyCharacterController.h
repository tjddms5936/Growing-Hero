// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "StructureCollection.h"
#include "MyCharacterController.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDele_Dynamic);
DECLARE_DELEGATE(FDele_Single);

UCLASS()
class GROWINGHERO_API AMyCharacterController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMyCharacterController();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AActor* m_pClickedProp;

	UPROPERTY(BlueprintReadWrite)
	class AMyCharacter* m_pMyHero;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FDele_Dynamic ED_ItemPickupPressed; // 블루프린트에서 호출하고 아이템이 바인딩

	FDele_Single ED_EnemyOutlineOff; // EnemyCharacter에서 호출 바인딩 다함.

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FDele_Dynamic ED_NPC_Conversation; // 키 누르면 호출하고 NPC 블루프린트에서 바인딩

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bClickMouse;
private:
	float m_fInterfaceRagne;

private:
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void BeginPlay() override;

public:
	// 키 입력 바이딩 함수
	UFUNCTION(BlueprintCallable)
	void InputClickPressed();

	UFUNCTION(BlueprintCallable)
	void InputClickReleased();

	UFUNCTION(BlueprintCallable)
	void InventoryToggle();

	UFUNCTION(BlueprintCallable)
	void EquipmentWindowToggle();

	UFUNCTION(BlueprintCallable)
	void SkillWindowToggle();

	UFUNCTION(BlueprintCallable)
	void StatWindowToggle();

	UFUNCTION(BlueprintCallable)
	void HotKeyPressed(EKEY eKey);

	UFUNCTION(BlueprintCallable)
	void NPC_ConversationKeyPressed();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void GameMenuToggle();

	UFUNCTION(BlueprintCallable)
	void OpenedFrameTearDownPressed();
private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void InitComponentSetting();
	void LoadUMGSetting();

public:
	UFUNCTION(BlueprintCallable)
	AActor* getClickedActor();
	void AttackAgain();
	FVector getMouseLocation();
	void setClickedActor(AActor* pActor);
	void CharLevelUp();
public:
	// === inventory 작업 ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UInventoryComponent* InventorySystemComponent;

	// === HotKey 작업 ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UHotKeyComponent* HotKeyComponent;

	// === Equipment 작업 ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UEquipmentComponent* EquipmentComponent;

	// === Skill 작업 ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkillComponent* SkillComponent;

	// === Stat 작업 ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStatComponent* StatComponent;

	// === 카메라 투명도 작업 ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraTransparencyComponent* CameraTransparencyComponent;


	// === UMG ===
	TQueue<class UUMG_Frame*> m_arOpenedFrame;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Customizing|UMG")
	TSubclassOf<class UUserWidget> InventoryFrameClass;
	UPROPERTY(BlueprintReadOnly, Category = "Customizing|UMG")
	class UUMG_InventoryFrame* InventoryFrame;
	UFUNCTION(BlueprintCallable)
	void InventoryFrameRefresh(EItemType eItemType);
	UFUNCTION(BlueprintCallable)
	void LoadInventoryHotKey(const TMap<int32, int32>& mapData);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Customizing|UMG")
	TSubclassOf<class UUserWidget> EquipmentFrameClass;
	class UUMG_EquipmentFrame* EquipmentFrame;
	UFUNCTION(BlueprintCallable)
	void EquipmentFrameRefresh();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Customizing|UMG")
	TSubclassOf<class UUserWidget> SkillFrameClass;
	UPROPERTY(BlueprintReadOnly, Category = "Customizing|UMG")
	class UUMG_SkillFrame* SkillFrame;
	UFUNCTION(BlueprintCallable)
	void SkillFrameRefresh();
	UFUNCTION(BlueprintCallable)
	void LoadSkillHotKey(const TMap<int32, int32>& mapData);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Customizing|UMG")
	TSubclassOf<class UUserWidget> StatFrameClass;
	UPROPERTY(BlueprintReadOnly)
	class UUMG_StatFrame* StatFrame;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Customizing|UMG")
	TSubclassOf<class UUserWidget> GameHUDClass;
	UPROPERTY(BlueprintReadOnly)
	class UUMG_GameHUD* GameHUD;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Customizing|UMG")
	TSubclassOf<class UUserWidget> HotKeyWindowClass;
	UPROPERTY(BlueprintReadOnly)
	class UUMG_HotKeyWindow* HotKeyWindow;
	UFUNCTION(BlueprintCallable)
	void LoadHotKeyInfo();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void GameAndUIModeSetting();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void GameOnlyModeSetting();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UIOnlyModeSetting();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Customizing|UMG")
	TSubclassOf<class UUserWidget> BackGroundClass;
	UPROPERTY(BlueprintReadOnly, Category = "Customizing|UMG")
	class UUserWidget* BackGround;

	UFUNCTION()
	void OpenChatBox();
	UFUNCTION()
	void CloseChatBox();

	void AddChatMsg(Protocol::S_CHAT& pkt, bool IsMine);
private:
	// For Server 
	class UMainGameInstance* GameIns;
	const float MOVE_PACKET_SEND_DELAY = 0.2f;
	float MovePacketSendTimer = MOVE_PACKET_SEND_DELAY;

	// Cache
	FVector ForwardDirection;
	FVector RightDirection;
	FVector2D DesiredInput;
	FVector DesiredMoveDirection;
	float DesiredYaw;


	// Dirty Flag Test
	FVector2D LastDesiredInput;
};
