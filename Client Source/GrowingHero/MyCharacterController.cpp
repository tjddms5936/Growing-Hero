// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterController.h"

#include "InventoryComponent.h"
#include "HotKeyComponent.h"
#include "EquipmentComponent.h"
#include "SkillComponent.h"
#include "StatComponent.h"
#include "CameraTransparencyComponent.h"

#include "EnemyCharacter.h"
#include "MyCharacter.h"
#include "MainGameInstance.h"
#include "QuestMGR.h"
#include "ServerSession.h"
#include "GrowingHero.h"

#include "UMG//UMG_InventoryFrame.h"
#include "UMG/UMG_EquipmentFrame.h"
#include "UMG/UMG_SkillFrame.h"
#include "UMG/UMG_StatFrame.h"
#include "UMG/UMG_GameHUD.h"
#include "UMG/UMG_HotKeyWindow.h"

#include "Kismet/KismetMathLibrary.h"

AMyCharacterController::AMyCharacterController() :
	bClickMouse{},
	m_fInterfaceRagne{},
	m_arOpenedFrame{}
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	InventorySystemComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventorySystemComponent"));
	HotKeyComponent = CreateDefaultSubobject<UHotKeyComponent>(TEXT("HotKeyComponent"));
	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipmentComponent"));
	SkillComponent = CreateDefaultSubobject<USkillComponent>(TEXT("SkillComponent"));
	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
	CameraTransparencyComponent = CreateDefaultSubobject<UCameraTransparencyComponent>(TEXT("CameraTransparencyComponent"));

	m_fInterfaceRagne = 10.f;

	DefaultMouseCursor = EMouseCursor::Default;
	CurrentMouseCursor = EMouseCursor::Default;
}

void AMyCharacterController::SetupInputComponent()
{
	EnableInput(this);
	Super::SetupInputComponent();
	
	if (InputComponent)
	{
		InputComponent->BindAction("MouseLeftClick", IE_Pressed, this, &AMyCharacterController::InputClickPressed);
		InputComponent->BindAction("MouseLeftClick", IE_Released, this, &AMyCharacterController::InputClickReleased);
		InputComponent->BindAction("InventoryToggle", IE_Pressed, this, &AMyCharacterController::InventoryToggle);
		InputComponent->BindAction("EquipmentWindowToggle", IE_Pressed, this, &AMyCharacterController::EquipmentWindowToggle);
		InputComponent->BindAction("SkillWindowToggle", IE_Pressed, this, &AMyCharacterController::SkillWindowToggle);
		InputComponent->BindAction("StatWindowToggle", IE_Pressed, this, &AMyCharacterController::StatWindowToggle);
		InputComponent->BindAction("OpendFrameTearDown", IE_Pressed, this, &AMyCharacterController::OpenedFrameTearDownPressed);
		InputComponent->BindAction("ChatOn", IE_Pressed, this, &AMyCharacterController::OpenChatBox);
		InputComponent->BindAction("ChatOff", IE_Pressed, this, &AMyCharacterController::CloseChatBox);
	
		DECLARE_DELEGATE_OneParam(FCustomInputDelegate, const EKEY);
		InputComponent->BindAction<FCustomInputDelegate>("HOTKEY_1", IE_Pressed, this, &AMyCharacterController::HotKeyPressed, EKEY::E_1);
		InputComponent->BindAction<FCustomInputDelegate>("HOTKEY_2", IE_Pressed, this, &AMyCharacterController::HotKeyPressed, EKEY::E_2);
		InputComponent->BindAction<FCustomInputDelegate>("HOTKEY_3", IE_Pressed, this, &AMyCharacterController::HotKeyPressed, EKEY::E_3);
		InputComponent->BindAction<FCustomInputDelegate>("HOTKEY_4", IE_Pressed, this, &AMyCharacterController::HotKeyPressed, EKEY::E_4);
		InputComponent->BindAction<FCustomInputDelegate>("HOTKEY_5", IE_Pressed, this, &AMyCharacterController::HotKeyPressed, EKEY::E_5);
		InputComponent->BindAction<FCustomInputDelegate>("HOTKEY_6", IE_Pressed, this, &AMyCharacterController::HotKeyPressed, EKEY::E_6);

		InputComponent->BindAction("NPC_Conversation", IE_Pressed, this, &AMyCharacterController::NPC_ConversationKeyPressed);
		

		InputComponent->BindAxis("MoveForward" ,this, &AMyCharacterController::MoveForward);
		// InputComponent->BindAxis("MoveRight", this, &AMyCharacterController::MoveForward);
		InputComponent->BindAxis("MoveRight", this, &AMyCharacterController::MoveRight);
	}
}



void AMyCharacterController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	bool ForceSendPacket = false;
	if (LastDesiredInput != DesiredInput)
	{
		ForceSendPacket = true;
		LastDesiredInput = DesiredInput;
	}

	// State정보. 키 안눌렀으면 Idle
	if (DesiredInput == FVector2D::Zero())
		m_pMyHero->m_PlayerInfo->set_state(Protocol::PLAYER_STATE_NONE);
	else
		m_pMyHero->m_PlayerInfo->set_state(Protocol::PLAYER_STATE_RUN);

	if (m_pMyHero->m_DashIdx < 4)
	{
		m_pMyHero->m_PlayerInfo->set_state(Protocol::PLAYER_STATE_DASH);
		ForceSendPacket = true;
		DesiredYaw = m_pMyHero->m_DesiredDashYaw;
	}

	// MovePacketSendTimer가 0이되면 보내고 리셋
	MovePacketSendTimer -= DeltaTime;
	if (((MovePacketSendTimer <= 0.f || ForceSendPacket) && GameIns && GameIns->m_bIsEnteredRoom))
	{
		MovePacketSendTimer = MOVE_PACKET_SEND_DELAY;

		Protocol::C_MOVE MovePkt;
		// 무엇을 채워줄것이냐. 현재 위치 & 내 ID. 서버가 내 ID제외하고 같은 Room 모든 클라한테 뿌려야함 내 이동정보를
		{
			// mutable이라는건 수정할 수 있다는것.
			{
				FVector CurLocation = m_pMyHero->GetActorLocation();
				m_pMyHero->m_PlayerInfo->set_x(CurLocation.X);
				m_pMyHero->m_PlayerInfo->set_y(CurLocation.X);
				m_pMyHero->m_PlayerInfo->set_z(CurLocation.Z);
			}

			Protocol::PlayerInfo* Info = MovePkt.mutable_info();
			Info->CopyFrom(*m_pMyHero->m_PlayerInfo);

			Info->set_yaw(DesiredYaw); // 더 자연스러움 위해 
			MovePkt.set_dashinfo(m_pMyHero->m_DashIdx);
		}
		TSharedPtr<SendBuffer> sendbuffer = ServerPacketHandler::MakeSendBuffer(MovePkt);
		GameIns->SendPacket(sendbuffer);
		m_pMyHero->m_DashIdx = 4;
	}
}


void AMyCharacterController::BeginPlay()
{
	Super::BeginPlay();
	FInputModeGameAndUI InputModeData;
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);

	m_pMyHero = Cast<AMyCharacter>(GetPawn());
	
	GameIns = Cast<UMainGameInstance>(GetGameInstance());
	if (IsValid(GameIns) && IsValid(GameIns->m_QuestMGR))
	{
		GameIns->m_QuestMGR->m_pMyController = this;
		GameIns->m_QuestMGR->m_pMyHero = m_pMyHero;
		GameIns->m_QuestMGR->m_world = GetWorld();

		GameIns->m_pMyController = this;
	}

	InitComponentSetting();
	LoadUMGSetting();
}


void AMyCharacterController::InputClickPressed()
{
	bClickMouse = true;
}

void AMyCharacterController::InputClickReleased()
{
	bClickMouse = false;
}

void AMyCharacterController::InventoryToggle()
{
	InventoryFrame->ToggleFrame();
	if (InventoryFrame->m_bOpenFrame)
	{
		m_arOpenedFrame.Enqueue(InventoryFrame);
	}
}

void AMyCharacterController::EquipmentWindowToggle()
{
	EquipmentFrame->ToggleFrame();
	if (EquipmentFrame->m_bOpenFrame)
	{
		m_arOpenedFrame.Enqueue(EquipmentFrame);
	}
}

void AMyCharacterController::SkillWindowToggle()
{
	SkillFrame->ToggleFrame();
	if (SkillFrame->m_bOpenFrame)
	{
		m_arOpenedFrame.Enqueue(SkillFrame);
	}
}

void AMyCharacterController::StatWindowToggle()
{
	StatFrame->ToggleFrame();
	if (StatFrame->m_bOpenFrame)
	{
		m_arOpenedFrame.Enqueue(StatFrame);
	}
}

void AMyCharacterController::HotKeyPressed(EKEY eKey)
{
	HotKeyComponent->ActivateHotKey(eKey);
}

void AMyCharacterController::NPC_ConversationKeyPressed()
{
	// Bind, UnBind는 NPC가 처리하고 여기서는 키 누르면 연결된 바인딩 함수 호출만 한다.
	if (ED_NPC_Conversation.IsBound() == true)
	{
		ED_NPC_Conversation.Broadcast(); // NPC 범위 내에 들어가있다면 NPC가 바인딩 했을 것이다.
	}
}

void AMyCharacterController::OpenedFrameTearDownPressed()
{
	UUMG_Frame* pTmpFrame{};
	while (!m_arOpenedFrame.IsEmpty())
	{
		m_arOpenedFrame.Dequeue(pTmpFrame);
		if (pTmpFrame->m_bOpenFrame)
		{
			pTmpFrame->ToggleFrame();
			break;
		}
	}
}

void AMyCharacterController::MoveForward(float Value)
{
	if (m_pMyHero->getUnitState() == EUNIT_STATE::E_Attack ||
		m_pMyHero->getUnitState() == EUNIT_STATE::E_Dead ||
		m_pMyHero->getUnitState() == EUNIT_STATE::E_UnderAttack)
		return;

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	// 카메라 전환되면 자연스럽게 캐릭터도 그 방향 봄
	ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	m_pMyHero->AddMovementInput(ForwardDirection, Value);
	// Cache for server
	{
		DesiredInput.Y = Value;

		DesiredMoveDirection = FVector::ZeroVector;
		DesiredMoveDirection += ForwardDirection * DesiredInput.Y;
		DesiredMoveDirection += RightDirection * DesiredInput.X;
		DesiredMoveDirection.Normalize();

		const FVector Location = m_pMyHero->GetActorLocation();
		FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(Location, Location + DesiredMoveDirection);
		DesiredYaw = Rotator.Yaw;
	}

}

void AMyCharacterController::MoveRight(float Value)
{
	if (m_pMyHero->getUnitState() == EUNIT_STATE::E_Attack ||
		m_pMyHero->getUnitState() == EUNIT_STATE::E_Dead ||
		m_pMyHero->getUnitState() == EUNIT_STATE::E_UnderAttack)
		return;

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	// 카메라 전환되면 자연스럽게 캐릭터도 그 방향 봄
	RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	m_pMyHero->AddMovementInput(RightDirection, Value);
	// Cache for server
	{
		DesiredInput.X = Value;

		DesiredMoveDirection = FVector::ZeroVector;
		DesiredMoveDirection += ForwardDirection * DesiredInput.Y;
		DesiredMoveDirection += RightDirection * DesiredInput.X;
		DesiredMoveDirection.Normalize();

		const FVector Location = m_pMyHero->GetActorLocation();
		FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(Location, Location + DesiredMoveDirection);
		DesiredYaw = Rotator.Yaw;
	}
}

void AMyCharacterController::InitComponentSetting()
{
	if (InventorySystemComponent)
		InventorySystemComponent->init();
	if (EquipmentComponent)
		EquipmentComponent->init();
	if (SkillComponent)
		SkillComponent->init();
	if (StatComponent)
		StatComponent->init(m_pMyHero);
	if (CameraTransparencyComponent)
		CameraTransparencyComponent->init(Cast<USpringArmComponent>(m_pMyHero->GetComponentByClass(USpringArmComponent::StaticClass())),
			Cast<UCameraComponent>(m_pMyHero->GetComponentByClass(UCameraComponent::StaticClass())),
			Cast<UCapsuleComponent>(m_pMyHero->GetComponentByClass(UCapsuleComponent::StaticClass())),
			m_pMyHero);
}

void AMyCharacterController::LoadUMGSetting()
{
	BackGround = CreateWidget<UUserWidget>(GetWorld(), BackGroundClass, FName(BackGroundClass->GetName()));
	if (BackGround)
	{
		BackGround->AddToViewport();
		BackGround->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}


	InventoryFrame = CreateWidget<UUMG_InventoryFrame>(GetWorld(), InventoryFrameClass, FName(InventoryFrameClass->GetName()));
	if (InventoryFrame)
	{
		InventoryFrame->AddToViewport();
		InventoryFrame->SetVisibility(ESlateVisibility::Hidden);
		InventoryFrame->init();
	}

	EquipmentFrame = CreateWidget<UUMG_EquipmentFrame>(GetWorld(), EquipmentFrameClass, FName(EquipmentFrameClass->GetName()));
	if (EquipmentFrame)
	{
		EquipmentFrame->AddToViewport();
		EquipmentFrame->SetVisibility(ESlateVisibility::Hidden);
		EquipmentFrame->init();
	}

	SkillFrame = CreateWidget<UUMG_SkillFrame>(GetWorld(), SkillFrameClass, FName(SkillFrameClass->GetName()));
	if (SkillFrame)
	{
		SkillFrame->AddToViewport();
		SkillFrame->SetVisibility(ESlateVisibility::Hidden);
		SkillFrame->init();
	}

	StatFrame = CreateWidget<UUMG_StatFrame>(GetWorld(), StatFrameClass, FName(StatFrameClass->GetName()));
	if (StatFrame)
	{
		StatFrame->AddToViewport();
		StatFrame->SetVisibility(ESlateVisibility::Hidden);
		StatFrame->init();
	}


	GameHUD = CreateWidget<UUMG_GameHUD>(GetWorld(), GameHUDClass, FName(GameHUDClass->GetName()));
	if (GameHUD)
	{
		GameHUD->AddToViewport();
		GameHUD->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		GameHUD->SetRenderOpacity(0.f);
		GameHUD->init();
	}

	HotKeyWindow = CreateWidget<UUMG_HotKeyWindow>(GetWorld(), HotKeyWindowClass, FName(HotKeyWindowClass->GetName()));
	if (HotKeyWindow)
	{
		HotKeyWindow->AddToViewport();
		HotKeyWindow->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		HotKeyWindow->SetRenderOpacity(0.f);
		HotKeyWindow->init();
	}
}

AActor* AMyCharacterController::getClickedActor()
{
	return m_pClickedProp;
}

void AMyCharacterController::AttackAgain()
{
	if (!IsValid(m_pClickedProp) || !IsValid(m_pMyHero) || m_pMyHero->getUnitState() == EUNIT_STATE::E_Dead)
		return;
	
	if (!m_pClickedProp->ActorHasTag("Enemy"))
		return;
	
	UE_LOG(LogTemp, Warning, TEXT("AttackAgain"));
	AEnemyCharacter* pEnemy = Cast<AEnemyCharacter>(m_pClickedProp);
	m_pMyHero->AttackEnemy(pEnemy);
}

FVector AMyCharacterController::getMouseLocation()
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, false, Hit);

	return Hit.Location;
}

void AMyCharacterController::setClickedActor(AActor* pActor)
{
	if (m_pMyHero->getUnitState() == EUNIT_STATE::E_Dead)
	{
		m_pClickedProp = nullptr;
		return;
	}
	m_pClickedProp = pActor;
}

void AMyCharacterController::CharLevelUp()
{
	StatComponent->AddStatPoint(5);
	StatComponent->StatUp(EStatName::E_Level, 1.f);
	StatComponent->UpdateStat();

	SkillComponent->AddSkillPoint(1);
}

void AMyCharacterController::InventoryFrameRefresh(EItemType eItemType)
{
	InventoryFrame->UpdateInventoryWindow(eItemType);
}

void AMyCharacterController::LoadInventoryHotKey(const TMap<int32, int32>& mapData)
{
	InventoryFrame->LoadInventoryHotKey(mapData);
}

void AMyCharacterController::EquipmentFrameRefresh()
{
	EquipmentFrame->UpdateEquipmentWindow();
}

void AMyCharacterController::SkillFrameRefresh()
{
	SkillFrame->UpdateSkillWindow();
}

void AMyCharacterController::LoadSkillHotKey(const TMap<int32, int32>& mapData)
{
	SkillFrame->LoadSkillHotKey(mapData);
}

void AMyCharacterController::LoadHotKeyInfo()
{
	HotKeyWindow->LoadHotKey();
}

void AMyCharacterController::OpenChatBox()
{
	if (GameHUD)
	{
		if (!GameHUD->m_bIsChatOn)
		{
			if (m_pMyHero->m_bIsAdventureMode)
				GameAndUIModeSetting();
			GameHUD->E_ChatOn();
		}
		else
		{
			GameHUD->FocusChat();
		}
	}
}

void AMyCharacterController::CloseChatBox()
{
	if (GameHUD && GameHUD->m_bIsChatOn)
	{
		GameHUD->E_ChatOff();
		GameAndUIModeSetting();
		GameHUD->SetFocus();
		// 모드에 따라 GameInputMode 변경해주기..
		if (m_pMyHero->m_bIsAdventureMode)
			GameOnlyModeSetting();
		
	}
}

void AMyCharacterController::AddChatMsg(Protocol::S_CHAT& pkt, bool IsMine)
{
	GameHUD->AddChatMsg(pkt, IsMine);
}
