#pragma once
#pragma warning(disable : 4984)
#include "GameSession.h"
#include <type_traits>
#include "Protocol.pb.h"

class ChildSessionMGR
{
public:
	ChildSessionMGR() = default;
	~ChildSessionMGR() = default;
	ChildSessionMGR(const ChildSessionMGR&) = delete;
	ChildSessionMGR& operator=(const ChildSessionMGR&) = delete;

public:
	template <typename T>
	void AddSession(std::shared_ptr<T> session);

    template <typename T>
    void ReleaseSession(std::shared_ptr<T> session);

    //  Broadcast to all Sessions in all rooms
    void Broadcast(shared_ptr<SendBuffer> sendBuffer, uint32 execeptID = 0);

    // Broadcast to all Sessions in only one room.
    void BroadcastRoom(shared_ptr<SendBuffer> sendBuffer, uint64 execeptID = 0, Protocol::RoomType eRoomType = Protocol::RoomType::ROOM_TUTORIAL);

    // 1:1 Send
    void OneToOneSend(shared_ptr<SendBuffer> sendBuffer, int32 SenderSessionID);

    shared_ptr<GameSession> GetSession(int32 sessionID) { return m_mapGameSession[sessionID]; }
private:
	// set<shared_ptr<GameSession>> m_setGameSessions;
    map<int32, shared_ptr<GameSession>> m_mapGameSession;

    mutex m;
};

extern ChildSessionMGR GChildSessionMGR;

template<typename T>
inline void ChildSessionMGR::AddSession(std::shared_ptr<T> session)
{
    lock_guard<mutex> guard(m);
    shared_ptr<IOCP_Session> Session = static_pointer_cast<IOCP_Session>(session);
    if (Session == nullptr)
        return;

    // T를 shared_ptr로 변환하여 적절한 컨테이너에 추가
    if(Session->GetSessionType() == IOCP_Session::SessionType::GAME_SESSION)
    {
        // T가 GameSession과 동일한 경우
        shared_ptr<GameSession> Session = static_pointer_cast<GameSession>(session);
        // m_setGameSessions.insert(static_pointer_cast<GameSession>(session));
        m_mapGameSession.insert({ Session->GetSessionID(), Session });
        cout << "New session ID : " << Session->GetSessionID() << "\n";
        cout << "AddSession. Session Cnt : " << m_mapGameSession.size() << "\n";
    }
    else 
    {
        // 다른 세션 타입에 대한 처리
        // 추가적인 세션 타입이 있다면 여기에 추가
    }
}

template<typename T>
inline void ChildSessionMGR::ReleaseSession(std::shared_ptr<T> session)
{
    lock_guard<mutex> guard(m);

    shared_ptr<IOCP_Session> Session = static_pointer_cast<IOCP_Session>(session);
    if (Session == nullptr)
        return;

    // T를 shared_ptr로 변환하여 적절한 컨테이너에 추가
    if (Session->GetSessionType() == IOCP_Session::SessionType::GAME_SESSION)
    {
        // T가 GameSession과 동일한 경우
        // m_setGameSessions.erase(static_pointer_cast<GameSession>(session));
        m_mapGameSession.erase(Session->GetSessionID());
        cout << "Remained Session Cnt : " << m_mapGameSession.size() << "\n";
    }
    else
    {
        // 다른 세션 타입에 대한 처리
        // 추가적인 세션 타입이 있다면 여기에 추가
    }
}
