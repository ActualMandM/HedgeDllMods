#include "ArchiveTreePatcher.h"
#include "Configuration.h"

extern "C" __declspec(dllexport) void Init(ModInfo* modInfo)
{
	// Disable Title Loading Video by HyperBE32
	WRITE_MEMORY(0xD6966E, uint8_t, 0xE9, 0x14, 0x01, 0x00, 0x00);

	// read configuration file
	std::string dir = modInfo->CurrentMod->Path;

	size_t pos = dir.find_last_of("\\/");
	if (pos != std::string::npos)
		dir.erase(pos + 1);

	if (Configuration::load(dir + "LinkSonic.ini"))
		return;

	MessageBox(nullptr, TEXT("Failed to load the config file!\nPlease make sure that LinkSonic.ini exists in the mod's folder."), TEXT("Link Sonic"), MB_ICONERROR);
}

extern "C" __declspec(dllexport) void PostInit(ModInfo* mods)
{
	// check for loaded mods
	for (Mod* mod : *mods->ModList)
	{
		if (!_stricmp(mod->Name, "eggmanland"))
			ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("LinkSonicEGB", { "cpz200" }));
	}

	// TunicType configuration
	switch (Configuration::tunicType)
	{
		case TunicType::Red:
			ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("AppearanceRed", { "LinkSonic" }));
			break;
		case TunicType::Blue:
			ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("AppearanceBlue", { "LinkSonic" }));
			break;
		case TunicType::Purple:
			ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("AppearancePurple", { "LinkSonic" }));
			break;
		default:
			break;
	}

	ArchiveTreePatcher::applyPatches();
}
