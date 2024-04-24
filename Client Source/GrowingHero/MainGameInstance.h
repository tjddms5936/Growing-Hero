// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Protocol.pb.h"
#include "MainGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMainGameInstance(const FObjectInitializer& ObjectInitializer);

public:
	virtual void Init() override;
	virtual void Shutdown() override;


public:
	UFUNCTION(BlueprintCallable)
	void ConnectServer(const FText& NewPlayerName);

	UFUNCTION(BlueprintCallable)
	void DisconnectServer();

	UFUNCTION(BlueprintCallable)
	void MonitorRecvPackets();

	// 1:Tutorial 2:Dungeon1
	UFUNCTION(BlueprintCallable)
	void EnterRoom(int32 RoomNumber);

	UFUNCTION(BlueprintCallable)
	void LeaveRoom();

	void SendPacket(TSharedPtr<class SendBuffer> SendBuffer);

	
public:
	// PacketHandler로부터 들어온 일감은 다 아래 함수로 정의해서 떠넘겨준다. 
	
	void Work_S_LOGIN(Protocol::S_LOGIN& pkt);
	void Work_S_ENTER_ROOM(Protocol::S_ENTER_ROOM& pkt);
	void Work_S_SPAWN(Protocol::S_SPAWN& pkt);
	void Work_S_DESPAWN(Protocol::S_DESPAWN& pkt);
	void Work_S_MOVE(Protocol::S_MOVE& pkt);
	void Work_S_CHAT(Protocol::S_CHAT& pkt);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UQuestMGR* m_QuestMGR;

public:
	
	// GameServer
	class FSocket* Socket;
	FString IpAddress = TEXT("3.35.229.110"); // AWS EC2 
	// FString IpAddress = TEXT("127.0.0.1"); // 루프백 어드레스
	int16 Port = 7777;
	TSharedPtr<class ServerSession> m_ServerSession;
	class AMyCharacter* m_pMyHero;
	class AMyCharacterController* m_pMyController;
	FString PlayerName;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AUnitBase> m_OtherPlayerClass;

	TMap<uint64, AUnitBase*> m_Players;

	// ================ 채팅 관련 =====================


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool m_bIsConnected;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool m_bIsEnteredRoom;
private:
	class UWorld* m_World;
};
