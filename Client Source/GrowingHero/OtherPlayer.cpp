// Fill out your copyright notice in the Description page of Project Settings.


#include "OtherPlayer.h"
#include "Engine/Classes/Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Protocol.pb.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MainGameInstance.h"
#include "ServerSession.h"
#include "AIController.h"

AOtherPlayer::AOtherPlayer()
{
	// ƽ �Լ��� �����ϵ��� ����
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// ĳ���� Ŭ������ �⺻������ ������ �ִ� ĸ�� �ݶ��̴��� ũ�⸦ �ʱ�ȭ
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 640.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;


	DashNiagara = CreateDefaultSubobject<UNiagaraComponent >(TEXT("DashNiagara"));
	DashNiagara->SetupAttachment(RootComponent);
	DashNiagara->bAutoActivate = false;

	m_eCharType = ECharType::E_Hero;
}

AOtherPlayer::~AOtherPlayer()
{
	if (m_PlayerInfo == nullptr || m_DestInfo == nullptr)
		return;
	delete m_PlayerInfo;
	delete m_DestInfo;
	m_PlayerInfo = nullptr;
	m_DestInfo = nullptr;
}

void AOtherPlayer::BeginPlay()
{
	Super::BeginPlay();

	{
		FVector Location = GetActorLocation();
		m_DestInfo->set_x(Location.X);
		m_DestInfo->set_y(Location.Y);
		m_DestInfo->set_z(Location.Z);
		m_DestInfo->set_yaw(GetControlRotation().Yaw);

		SetMoveState(Protocol::PLAYER_STATE_NONE);
	}
}

void AOtherPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// ȸ�� ���� �� ������ �ϰ� ������... ���� ������ ��� ���� ��.
	const Protocol::PlayerState State = m_PlayerInfo->state();
	switch (State)
	{
	case Protocol::PLAYER_STATE_RUN:
	{
		SetActorRotation(FRotator(0, m_DestInfo->yaw(), 0));
		const FVector Direction = GetActorForwardVector();
		AddMovementInput(Direction, 1.0f);
		break;
	}
	case Protocol::PLAYER_STATE_DASH:
	{
		SetActorRotation(FRotator(0, m_DestInfo->yaw(), 0));
		DashStart(m_DashIdx);
		break;
	}
	case Protocol::PLAYER_STATE_ATTACK:
	{

		break;
	}
	case Protocol::PLAYER_STATE_HITTED:
	{

		break;
	}
	case Protocol::PLAYER_STATE_DIE:
	{

		break;
	}
	case Protocol::PLAYER_STATE_STUN:
	{

		break;
	}
	case Protocol::PLAYER_STATE_JUMP:
	{

		break;
	}
	default:
		break;
	}
}

void AOtherPlayer::SetMoveState(Protocol::PlayerState StateInfo)
{
	if (m_PlayerInfo->state() == StateInfo)
		return;

	m_PlayerInfo->set_state(StateInfo);
}


void AOtherPlayer::SetDestInfo(const Protocol::PlayerInfo& Info, int32 DashIdx)
{
	// Dest�� ���� ���� ����
	m_DestInfo->CopyFrom(Info);
	m_DashIdx = DashIdx;
	// ���¸�ŭ�� �ٷ� ��������
	SetMoveState(Info.state());
}

void AOtherPlayer::SetPlayerInfo(const Protocol::PlayerInfo& Info)
{
	Super::SetPlayerInfo(Info);
}
