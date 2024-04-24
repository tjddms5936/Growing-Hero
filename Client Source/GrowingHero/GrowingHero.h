// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ServerPacketHandler.h"
#include "MainGameInstance.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "NetworkThread.h"

#define SEND_PKT(pkt) \
	TSharedPtr<SendBuffer> sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt); \
	Cast<UMainGameInstance>(GWorld->GetGameInstance())->SendPacket(sendBuffer);

#define P_HEADER(buffer) \
	reinterpret_cast<FPacketHeader*>(buffer->GetBuffer());