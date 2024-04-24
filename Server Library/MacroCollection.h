#pragma once

// 인위적으로 Crash
#define CRASH()						\
{									\
uint32* crash = nullptr;			\
__analysis_assume(crash != nullptr);\
*crash = 0xABABABAB;				\
}

