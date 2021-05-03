#include "ArchiveTreePatcher.h"

extern "C" __declspec(dllexport) void Init()
{
	// Disable Title Loading Video by Hyper
	WRITE_MEMORY(0xD6966E, uint8_t, 0xE9, 0x14, 0x01, 0x00, 0x00);
}

extern "C" __declspec(dllexport) void PostInit()
{
	if (GetModuleHandle(TEXT("GenerationsPBRShaders.dll")) == nullptr)
		ArchiveTreePatcher::applyPatches();
}
