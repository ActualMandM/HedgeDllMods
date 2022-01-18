#include "ArchiveTreePatcher.h"
#include "Configuration.h"
#include "ShaderLoader.h"

extern "C" __declspec(dllexport) void Init()
{
	// Disable Title Loading Video by HyperBE32
	WRITE_MEMORY(0xD6966E, uint8_t, 0xE9, 0x14, 0x01, 0x00, 0x00);

	// check if the configuration file exists
	if (!Configuration::load("LinkSonic.ini"))
		MessageBox(nullptr, TEXT("Failed to load the config file!\nPlease make sure that LinkSonic.ini exists in the mod's folder."), TEXT("Link Sonic"), MB_ICONERROR);

	// only load ChrEye shaders if slwEyes are enabled
	if (Configuration::slwEyes)
	{
		ShaderLoader::applyPatches();
		ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SLWEyes", { "LinkSonic", "LinkSonicEGB" }));
	}

	// TunicType configuration
	switch (Configuration::tunicType)
	{
		default:
		// Green
			ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("AppearanceGreen", { "LinkSonic", "LinkSonicEGB", "PBRSonic" }));
			break;
		case Red:
			ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("AppearanceRed", { "LinkSonic", "LinkSonicEGB", "PBRSonic" }));
			break;
		case Blue:
			ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("AppearanceBlue", { "LinkSonic", "LinkSonicEGB", "PBRSonic" }));
			break;
		case Purple:
			ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("AppearancePurple", { "LinkSonic", "LinkSonicEGB", "PBRSonic" }));
			break;
		case Rainbow:
			ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("AppearanceRainbow", { "LinkSonic", "LinkSonicEGB" }));
			break;
	}

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
