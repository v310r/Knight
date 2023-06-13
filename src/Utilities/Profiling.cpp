#include "Profiling.h"

#include <iostream>


static AllocationMetrics g_AllocationMetrics;


void* operator new(size_t size)
{
	g_AllocationMetrics.AllocatedMemory += size;

	return malloc(size);
}

void operator delete(void* memory, size_t size)
{
	g_AllocationMetrics.FreedMemory += size;

	free(memory);
}

void PrintMemoryUsage(const char* message)
{
	std::cout << "Memory usage: " << g_AllocationMetrics.CurrentUsage() << " bytes " << message << "\n";
}