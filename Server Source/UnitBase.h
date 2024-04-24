#pragma once
#include "Protocol.pb.h"

class RoomBase;

class UnitBase : enable_shared_from_this<UnitBase>
{
public:
	UnitBase();
	virtual ~UnitBase();

public:
	void init();
	void UpdateInfo(const Protocol::PlayerInfo& info);
	const Protocol::PlayerInfo& GetPlayerInfo() { return *m_UnitInfo; }
	void SetPlayerName(const string& Name) { m_PlayerName = Name; }
	const string& GetPlayerName() { return m_PlayerName; }
private:
	Protocol::PlayerInfo* m_UnitInfo;
	string m_PlayerName;
};

