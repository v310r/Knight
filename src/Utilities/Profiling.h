#pragma once

#include <cstdint>
#include <cstdlib>


struct AllocationMetrics
{
	uint64_t AllocatedMemory = 0;

	uint64_t FreedMemory = 0;

	uint64_t CurrentUsage() { return AllocatedMemory - FreedMemory; }
};

void PrintMemoryUsage(const char* message, const char* spacing = "");

#ifdef _DEBUG

#define PROFILE_MEMORY_USAGE(_funcCall_)	PrintMemoryUsage("BEFORE {" #_funcCall_ "} CALL"); \
												_funcCall_ \
												PrintMemoryUsage("AFTER {" #_funcCall_ "} CALL", "\n");

#else #define PROFILE_MEMORY_USAGE(_funcCall_)
#endif