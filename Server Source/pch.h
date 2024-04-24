#pragma once

#define WIN32_LEAN_AND_MEAN           

#ifdef _DEBUG
#pragma comment(lib, "ServerLib\\Debug\\ServerLib.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "ServerLib\\Release\\ServerLib.lib")
#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib")
#endif

#include "MainPCH.h"