#include "ArchiveTreePatcher.h"
#include "Configuration.h"

vector<string> sonicArchives = { "ev031", "ev041", "ev042", "ev091", "evSonic", "Sonic", "Title" };

extern "C" __declspec(dllexport) void Init()
{
	if (!Configuration::load("ColorsSonic.ini"))
		MessageBox(nullptr, TEXT("Failed to load the config file!\nPlease make sure that ColorsSonic.ini exists in the mod's folder."), TEXT("Colors Sonic"), MB_ICONERROR);

	// SimulatorType configuration
	if (Configuration::enableSimulator)
	{
		ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SimulatorSonic", { sonicArchives }));

		// Simulator Animation Swap by HyperBE32
		WRITE_MEMORY(0x15E6848, uint8_t, 0x73, 0x69, 0x6D, 0x5F, 0x73, 0x74, 0x61, 0x72, 0x74, 0x5F, 0x77, 0x61, 0x69, 0x74, 0x5F, 0x61, 0x00); /* sim_start_wait_a */
		WRITE_MEMORY(0x15E6859, uint8_t, 0x73, 0x69, 0x6D, 0x5F, 0x73, 0x74, 0x61, 0x72, 0x74, 0x5F, 0x6E, 0x6F, 0x72, 0x6D, 0x61, 0x6C, 0x00); /* sim_start_normal */
		WRITE_MEMORY(0x1277C75, uint32_t, 0x015E6848);
		WRITE_MEMORY(0x1277C0E, uint32_t, 0x015E6859);

		switch (Configuration::simulatorType)
		{
			case SimulatorType::Blue:
				ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SimulatorBlue", { "SimulatorSonic" }));
				break;
			case SimulatorType::Black:
				ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SimulatorBlack", { "SimulatorSonic" }));
				break;
			case SimulatorType::Green:
				ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SimulatorGreen", { "SimulatorSonic" }));
				break;
			case SimulatorType::Pink:
				ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SimulatorPink", { "SimulatorSonic" }));
				break;
			case SimulatorType::Red:
				ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SimulatorRed", { "SimulatorSonic" }));
				break;
		}
	}

	ArchiveTreePatcher::applyPatches();
}