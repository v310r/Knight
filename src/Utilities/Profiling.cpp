#include "Profiling.h"

#include <iostream>


static AllocationMetrics g_AllocationMetrics;


//static void* operator new(size_t size)
//{
//	g_AllocationMetrics.AllocatedMemory += size;
//
//	return malloc(size);
//}
//
//static void operator delete(void* memory, size_t size)
//{
//	g_AllocationMetrics.FreedMemory += size;
//
//	free(memory);
//}

void PrintMemoryUsage(const char* message, const char* spacing)
{
	std::cout << "Memory usage: " << g_AllocationMetrics.CurrentUsage() << " bytes " << message << "\n" << spacing;
}