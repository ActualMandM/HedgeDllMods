#include "ArchiveTreePatcher.h"

extern "C" __declspec(dllexport) void Init()
{
	// Disable Title Loading Video by Hyper
	WRITE_MEMORY(0xD6966E, uint8_t, 0xE9, 0x14, 0x01, 0x00, 0x00);
}

extern "C" __declspec(dllexport) void PostInit(ModInfo* mods)
{

    // check for loaded mods
    for (Mod* mod : *mods->ModList)
    {
        if (!_stricmp(mod->Name, "eggmanland"))
            ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("LinkSonicEGB", { "cpz200" }));
    }

	ArchiveTreePatcher::applyPatches();
}
