#pragma once

// ���������� Crash
#define CRASH()						\
{									\
uint32* crash = nullptr;			\
__analysis_assume(crash != nullptr);\
*crash = 0xABABABAB;				\
}

