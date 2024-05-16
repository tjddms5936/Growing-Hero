// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"
#include "Common/TcpSocketBuilder.h"
#include "Serialization/ArrayWriter.h"
#include "SocketSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "SocketTypes.h"

#include "Protocol.pb.h"
#include "ServerSession.h"
#include "ServerPacketHandler.h"
#include "UnitBase.h"
#include "MyCharacter.h"
#include "MyCharacterController.h"
#include "QuestMGR.h"

UMainGameInstance::UMainGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), Socket{}, m_ServerSession{}, m_pMyHero(nullptr), m_pMyController(nullptr), m_bIsConnected(false), m_bIsEnteredRoom(false)
{
	m_QuestMGR = CreateDefaultSubobject<UQuestMGR>(TEXT("QuestMGR"));
	
}

void UMainGameInstance::Init()
{
	Super::Init();
}

void UMainGameInstance::Shutdown()
{
	Super::Shutdown();
	DisconnectServer();
}

void UMainGameInstance::ConnectServer(const FText& NewPlayerName)
{
	m_World = GetWorld();
	if (m_World == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No World Error..."));
		return;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Yes World Success!"));
	}

	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));
	FIPv4Address Ip;
	FIPv4Address::Parse(IpAddress, Ip);

	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(Ip.Value);
	InternetAddr->SetPort(Port);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connecting To Server...")));

	m_bIsConnected = Socket->Connect(*InternetAddr);

	if (m_bIsConnected)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Success")));

		// 일단 만들고 sessionID는 임의로 0 지정. 연결되면 부여받음
		m_ServerSession = MakeShared<ServerSession>(Socket);
		m_ServerSession->init();
		{
			Protocol::C_LOGIN pkt;

			// UTF-8 형식으로 변환
			FString TextString = NewPlayerName.ToString();
			std::string UTF8String = TCHAR_TO_UTF8(*TextString);
			pkt.set_playername(UTF8String);



			TSharedPtr<SendBuffer> sendbuffer = ServerPacketHandler::MakeSendBuffer(pkt);
			SendPacket(sendbuffer);
		}

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Failed")));
	}
}

void UMainGameInstance::DisconnectServer()
{
	if (m_ServerSession == nullptr)
		return;

	Protocol::C_DISCONNECT pkt{};
	pkt.set_player_id(m_ServerSession->GetSessionID());
	TSharedPtr<SendBuffer> sendbuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	SendPacket(sendbuffer);

	m_ServerSession->Release();


	if (Socket) // 여전히 유효한지 확인
	{
		ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get();
		SocketSubsystem->DestroySocket(Socket); // 소켓을 파괴
		return;
	}
}

void UMainGameInstance::MonitorRecvPackets()
{
	if (Socket == nullptr || m_ServerSession == nullptr || m_bIsConnected == false)
		return;

	m_ServerSession->MonitorRecvPackets();
}

void UMainGameInstance::EnterRoom(int32 RoomNumber)
{
	if (!m_bIsConnected)
		return;

	Protocol::C_ENTER_ROOM pkt;

	std::string UTF8String = TCHAR_TO_UTF8(*PlayerName);
	pkt.set_playername(UTF8String);

	Protocol::PlayerInfo* info = new Protocol::PlayerInfo();
	info->set_player_id(m_ServerSession->GetSessionID());
	{
		Protocol::RoomType room;
		room = Protocol::RoomType::ROOM_TUTORIAL;
		switch (RoomNumber)
		{
		case 0:
			room = Protocol::RoomType::ROOM_TUTORIAL;
			break;
		case 1:
			room = Protocol::RoomType::ROOM_FIRSTDUN;
			break;
		default:
			room = Protocol::RoomType::ROOM_TUTORIAL;
			break;
		}

		info->set_room(room);
	}
	pkt.set_allocated_info(info);

	TSharedPtr<SendBuffer> sendbuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	SendPacket(sendbuffer);

}

void UMainGameInstance::LeaveRoom()
{
	if (!m_bIsConnected)
		return;

	m_bIsEnteredRoom = false;
	m_Players.Reset();

	Protocol::C_LEAVE_ROOM pkt;
	Protocol::PlayerInfo* info = new Protocol::PlayerInfo();

	info->CopyFrom(m_pMyHero->GetPlayerInfo());
	pkt.set_allocated_info(info);

	TSharedPtr<SendBuffer> sendbuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	SendPacket(sendbuffer);
}

void UMainGameInstance::SendPacket(TSharedPtr<class SendBuffer> SendBuffer)
{
	if (Socket == nullptr || m_ServerSession == nullptr)
		return;

	m_ServerSession->SendToServer(SendBuffer);
}

void UMainGameInstance::Work_S_LOGIN(Protocol::S_LOGIN& pkt)
{
	// 로그인 success면 sessionID도 부여받아옴, 그걸 내 Session한테 너 ID 이거야 라고 부여하자.
	// Session 만들고 가동
	if (pkt.success())
	{
		m_ServerSession->SetSessionID(pkt.yoursessionid());
		PlayerName = UTF8_TO_TCHAR(pkt.playername().c_str());
	}
}

void UMainGameInstance::Work_S_ENTER_ROOM(Protocol::S_ENTER_ROOM& pkt)
{
	if (Socket == nullptr || m_ServerSession == nullptr || m_pMyController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Work_S_ENTER_ROOM Fail 1"));
		return;
	}

	Protocol::PlayerInfo info = pkt.info();
	// 중복 처리 체크
	const uint64 playerId = info.player_id();
	if (m_Players.Find(playerId) != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Work_S_ENTER_ROOM Fail 2"));
		return;
	}
	FVector InitLocation(info.x(), info.y(), info.z());

	if (m_pMyController->m_pMyHero == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Work_S_ENTER_ROOM Fail 3"));
		return;
	}

	m_bIsEnteredRoom = true;

	m_pMyHero = m_pMyController->m_pMyHero;
	m_Players.Add(info.player_id(), m_pMyHero);

	m_pMyHero->SetPlayerInfo(info);

	// 처음 Room 입장 시 시작 위치만 지정은 내 클라 & 다른 클라 공통.
	FVector NewLocation(info.x(), info.y(), info.z());
	m_pMyHero->SetActorLocation(NewLocation);
}

void UMainGameInstance::Work_S_SPAWN(Protocol::S_SPAWN& pkt)
{
	if (Socket == nullptr || m_ServerSession == nullptr)
		return;

	m_World = GetWorld();
	if (m_World == nullptr)
		return;


	for (Protocol::PlayerInfo player : pkt.info())
	{
		// 중복 처리 체크
		const uint64 playerId = player.player_id();
		UE_LOG(LogTemp, Warning, TEXT("Spawn ID : %d"), player.player_id());
		if (m_Players.Find(playerId) != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Spawn ID : %d  -> Fail. Already in map"), player.player_id());
			return;
		}
		
		FVector SpawnLocation(player.x(), player.y(), player.z());
		AUnitBase* OtherPlayer = Cast<AUnitBase>(m_World->SpawnActor(m_OtherPlayerClass, &SpawnLocation));
		if (OtherPlayer)
		{
			OtherPlayer->SetPlayerInfo(player);
			m_Players.Add(player.player_id(), OtherPlayer);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Spawn ID : %d  -> Fail"), player.player_id());
		}
	}
}

void UMainGameInstance::Work_S_DESPAWN(Protocol::S_DESPAWN& pkt)
{
	UE_LOG(LogTemp, Warning, TEXT("Work_S_DESPAWN : %d"), pkt.player_id());
	AUnitBase* pLeavePlayer = *m_Players.Find(pkt.player_id());
	if (pLeavePlayer != nullptr)
	{
		m_Players.Remove(pkt.player_id());
		pLeavePlayer->Disappear();
	}
}

void UMainGameInstance::Work_S_MOVE(Protocol::S_MOVE& pkt)
{
	if (Socket == nullptr || m_ServerSession == nullptr)
		return;

	m_World = GetWorld();
	if (m_World == nullptr)
		return;

	// pkt에는 누가 움직였는지 ID가 있을 것이다.
	const uint64 objectID = pkt.info().player_id();

	AUnitBase** FindObject = m_Players.Find(objectID);
	if (FindObject == nullptr)
		return;

	AUnitBase* MovingPlayer = (*FindObject);

	// 위에서 null체크했으니까 바로 Go
	const Protocol::PlayerInfo& Info = pkt.info();
	MovingPlayer->SetDestInfo(Info, pkt.dashinfo());


}

void UMainGameInstance::Work_S_CHAT(Protocol::S_CHAT& pkt)
{
	bool IsMine = false;
	if (pkt.senderinfo().player_id() == m_ServerSession->GetSessionID())
		IsMine = true;
	m_pMyController->AddChatMsg(pkt, IsMine);
}

