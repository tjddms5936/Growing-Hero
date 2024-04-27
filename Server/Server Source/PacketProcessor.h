#pragma once
#include "Protocol.pb.h"
#include <random>
#include <mutex>

// 전송받은 패킷들 처리하는 함수 모음 클래스
class PacketProcessor
{
public:
	PacketProcessor() = default;
	~PacketProcessor() = default;
	PacketProcessor(const PacketProcessor&) = delete;
	PacketProcessor& operator=(const PacketProcessor&) = delete;
public:
	// 서버 입장에서는 어떤 sessionID가 보냈는지도 알아야 함.
	static void Work_C_LOGIN(int32 sessionID, Protocol::C_LOGIN& pkt);
	static void Work_C_ENTER_ROOM(int32 sessionID, Protocol::C_ENTER_ROOM& pkt);
	static void Work_C_LEAVE_ROOM(int32 sessionID, Protocol::C_LEAVE_ROOM& pkt);
	static void Work_C_MOVE(Protocol::C_MOVE& pkt);
	static void Work_C_CHAT(Protocol::C_CHAT& pkt);
	static void Work_C_DISCONNECT(int32 sessionID, Protocol::C_LEAVE_ROOM& pkt);


	template<typename T>
	static T GetRandomNum(T min, T max);

	
};

extern PacketProcessor GPktProcessor;

template<typename T>
inline T PacketProcessor::GetRandomNum(T min, T max)
{
	// 시드값을 얻기 위한 random_device 생성.
	std::random_device randomDevice;
	// random_device 를 통해 난수 생성 엔진을 초기화 
	std::mt19937 generator(randomDevice());
	// 균등하게 나타나는 난수열을 생성하기 위해 균등 분포 정의.

	// constexpr을 사용하면 컴파일 타임에 만들어지고 둘 중 하나로 들어가게 해준다.
	if constexpr (std::is_integral_v<T>)
	{
		std::uniform_int_distribution<T> distribution(min, max);
		return distribution(generator);
	}
	else
	{
		std::uniform_real_distribution<T> distribution(min, max);
		return distribution(generator);
	}
}
