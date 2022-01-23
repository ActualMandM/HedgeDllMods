#include "ArchiveTreePatcher.h"
#include "Configuration.h"

static const char* tunicNames[] = { "Green", "Red", "Blue", "Purple", "Rainbow", "Slash", "Bash", "Foof", "Gray", "Pink", "Orange", "Black", "LightBlue" };

extern "C" __declspec(dllexport) void Init()
{
	// Disable Title Loading Video by HyperBE32
	WRITE_MEMORY(0xD6966E, uint8_t, 0xE9, 0x14, 0x01, 0x00, 0x00);

	// check if the configuration file exists
	if (!Configuration::load("LinkSonic.ini"))
		MessageBox(nullptr, TEXT("Failed to load the config file!\nPlease make sure that LinkSonic.ini exists in the mod's folder."), TEXT("Link Sonic"), MB_ICONERROR);

	// TunicType configuration
	if (Configuration::tunicType != Rainbow)
		ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("Appearance" + std::string(tunicNames[Configuration::tunicType]), { "LinkSonic", "LinkSonicEGB", "PBRSonic" }));
	else
		ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("AppearanceRainbow", {"LinkSonic", "LinkSonicEGB"}));
}

extern "C" __declspec(dllexport) void PostInit(ModInfo* mods)
{
	// check for loaded mods
	for (Mod* mod : *mods->ModList)
	{
		if (!_stricmp(mod->Name, "eggmanland"))
		{
			ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("LinkSonicEGB", { "cpz200" }));

			if (GetModuleHandle(TEXT("SLWEyes.dll")) != nullptr)
				ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SLWEyes", { "LinkSonicEGB" }));
		}
	}

	ArchiveTreePatcher::applyPatches();
}
