#include "ArchiveTreePatcher.h"
#include "Configuration.h"

vector<string> sonicArchives = { "ev031", "ev041", "ev042", "ev091", "evSonic", "Sonic", "Title" };

extern "C" __declspec(dllexport) void Init(ModInfo* modInfo)
{
	// read configuration file
	std::string dir = modInfo->CurrentMod->Path;

	size_t pos = dir.find_last_of("\\/");
	if (pos != std::string::npos)
		dir.erase(pos + 1);

	if (Configuration::load(dir + "ColorsSonic.ini"))
		return;

	MessageBox(nullptr, TEXT("Failed to load the config file!\nPlease make sure that ColorsSonic.ini exists in the mod's folder."), TEXT("Colors Sonic"), MB_ICONERROR);
}

extern "C" __declspec(dllexport) void PostInit(ModInfo* mods)
{
	// SimulatorType configuration
	switch (Configuration::simulatorType)
	{
		case SimulatorType::Blue:
			ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SimulatorBlue", { sonicArchives }));
			break;
		case SimulatorType::Black:
			ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SimulatorBlack", { sonicArchives }));
			break;
		case SimulatorType::Green:
			ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SimulatorGreen", { sonicArchives }));
			break;
		case SimulatorType::Pink:
			ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SimulatorPink", { sonicArchives }));
			break;
		case SimulatorType::Red:
			ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SimulatorRed", { sonicArchives }));
			break;
		default:
			break;
	}

	ArchiveTreePatcher::applyPatches();
}
