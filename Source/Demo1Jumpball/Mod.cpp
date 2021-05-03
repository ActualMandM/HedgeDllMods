#include "ArchiveTreePatcher.h"

extern "C" __declspec(dllexport) void Init()
{
	ArchiveTreePatcher::applyPatches();
}
