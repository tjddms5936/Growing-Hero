#pragma once
#include "Protocol.pb.h"
#include <random>
#include <mutex>
#include <string>
#include <codecvt>
#include <locale>

// ���۹��� ��Ŷ�� ó���ϴ� �Լ� ���� Ŭ����
class PacketProcessor
{
public:
	PacketProcessor() = default;
	~PacketProcessor() = default;
	PacketProcessor(const PacketProcessor&) = delete;
	PacketProcessor& operator=(const PacketProcessor&) = delete;
public:
	// ���� ���忡���� � sessionID�� ���´����� �˾ƾ� ��.
	static void Work_C_LOGIN(int32 sessionID, Protocol::C_LOGIN& pkt);
	static void Work_C_ENTER_ROOM(int32 sessionID, Protocol::C_ENTER_ROOM& pkt);
	static void Work_C_LEAVE_ROOM(int32 sessionID, Protocol::C_LEAVE_ROOM& pkt);
	static void Work_C_MOVE(Protocol::C_MOVE& pkt);
	static void Work_C_CHAT(Protocol::C_CHAT& pkt);
	static void Work_C_DISCONNECT(int32 sessionID, Protocol::C_LEAVE_ROOM& pkt);


	template<typename T>
	static T GetRandomNum(T min, T max);

private:
	static std::wstring utf8_to_wstring(const std::string& str)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.from_bytes(str);
	}
};

extern PacketProcessor GPktProcessor;

template<typename T>
inline T PacketProcessor::GetRandomNum(T min, T max)
{
	// �õ尪�� ��� ���� random_device ����.
	std::random_device randomDevice;
	// random_device �� ���� ���� ���� ������ �ʱ�ȭ 
	std::mt19937 generator(randomDevice());
	// �յ��ϰ� ��Ÿ���� �������� �����ϱ� ���� �յ� ���� ����.

	// constexpr�� ����ϸ� ������ Ÿ�ӿ� ��������� �� �� �ϳ��� ���� ���ش�.
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
