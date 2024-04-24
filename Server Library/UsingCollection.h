#pragma once
// 자주 사용되는 타입들 재정의
using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;

using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

using uchar = unsigned char;

struct PacketHeader
{
	uint16 PacketSize;
	uint16 Packet_Id;  // 프로토콜 ID. ex) 1=로그인, 2=이동요청
};
