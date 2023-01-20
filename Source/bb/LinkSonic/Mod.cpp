#include "ArchiveTreePatcher.h"
#include "Configuration.h"

extern "C" __declspec(dllexport) void Init()
{
	// Check if the configuration file exists
	if (!Configuration::load("LinkSonic.ini"))
	{
		MessageBox(nullptr, TEXT("Failed to load the config file!\nPlease make sure that LinkSonic.ini exists in the mod's folder."),
			TEXT("Link Sonic"), MB_ICONERROR);
	}

	// TunicType configuration
	ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("Appearance" + Configuration::tunicType,
		{ "LinkSonic", "LinkSonicEGB", "PBRSonic" }));
}

extern "C" __declspec(dllexport) void PostInit(ModInfo* mods)
{
	// Check for loaded mods
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
