#include "ArchiveTreePatcher.h"
#include "ShaderLoader.h"

extern "C" __declspec(dllexport) void Init()
{
	ShaderLoader::applyPatches();
}

extern "C" __declspec(dllexport) void PostInit(ModInfo* mods)
{
	ArchiveTreePatcher::applyPatches();
}
