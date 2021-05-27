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
		ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SimulatorVoice", { "SonicVoice" }));

		// Simulator Animation Swap by HyperBE32
		WRITE_MEMORY(0x15E6848, uint8_t, 0x73, 0x69, 0x6D, 0x5F, 0x73, 0x74, 0x61, 0x72, 0x74, 0x5F, 0x77, 0x61, 0x69, 0x74, 0x5F, 0x61, 0x00); /* sim_start_wait_a */
		WRITE_MEMORY(0x15E6859, uint8_t, 0x73, 0x69, 0x6D, 0x5F, 0x73, 0x74, 0x61, 0x72, 0x74, 0x5F, 0x6E, 0x6F, 0x72, 0x6D, 0x61, 0x6C, 0x00); /* sim_start_normal */
		WRITE_MEMORY(0x1277C75, uint32_t, 0x015E6848);
		WRITE_MEMORY(0x1277C0E, uint32_t, 0x015E6859);

		// Disable Character Action Voices by HyperBE32
		WRITE_MEMORY(0xDFC993, uint32_t, 0xFFFFFF); /* 3002_action01 (used for bouncing off physics objects) */
		WRITE_MEMORY(0xE4B684, uint32_t, 0xFFFFFF); /* 3002_action01 (used for tricks) */
		WRITE_MEMORY(0xE4B8E0, uint32_t, 0xFFFFFF); /* 3002_action01 (used for trick finish) */
		WRITE_MEMORY(0xE57E6F, uint32_t, 0xFFFFFF); /* 3002_action01 (used for jumping) */
		WRITE_MEMORY(0x11D7B8C, uint32_t, 0xFFFFFF); /* 3002_action01 (used for grinding) */
		WRITE_MEMORY(0x1117EF4, uint32_t, 0xFFFFFF); /* 3002_stumble */
		WRITE_MEMORY(0xE0248C, uint32_t, 0xFFFFFF); /* 3002_damage01 */
		WRITE_MEMORY(0xE02496, uint32_t, 0xFFFFFF); /* 3002_damage02 */
		WRITE_MEMORY(0xE024A0, uint32_t, 0xFFFFFF); /* 3002_damage03 */
		WRITE_MEMORY(0xE024AA, uint32_t, 0xFFFFFF); /* 3002_damage04 */
		WRITE_MEMORY(0xDF0B63, uint32_t, 0xFFFFFF); /* 3002_damage06 */
		WRITE_MEMORY(0xE023E3, uint32_t, 0xFFFFFF); /* 3002_dead (used for dying from enemies) */
		WRITE_MEMORY(0xE55D3D, uint32_t, 0xFFFFFF); /* 3002_dead (used for dying from falling) */
		WRITE_MEMORY(0x1118512, uint32_t, 0xFFFFFF); /* 3002_action03 (used for drift) */
		WRITE_MEMORY(0x1254E04, uint32_t, 0xFFFFFF); /* 3002_action03 (used for stomping) */
		WRITE_MEMORY(0xDF3710, uint32_t, 0xFFFFFF); /* 3002_action04 (used for homing success) */
		WRITE_MEMORY(0x1117CEB, uint32_t, 0xFFFFFF); /* 3002_action04 (used for boost) */
		WRITE_MEMORY(0x1117D09, uint32_t, 0xFFFFFF); /* 3002_action04 (used for boost) */
		WRITE_MEMORY(0x1117D27, uint32_t, 0xFFFFFF); /* 3002_action04 (used for boost) */
		WRITE_MEMORY(0x11184E4, uint32_t, 0xFFFFFF); /* 3002_action04 (used for homing success) */
		WRITE_MEMORY(0xE4466E, uint32_t, 0xFFFFFF); /* 3002_strain (used for pushing) */
		WRITE_MEMORY(0xE3DADD, uint32_t, 0xFFFFFF); /* 3002_success01 (used for blast off) */
		WRITE_MEMORY(0xE462AA, uint32_t, 0xFFFFFF); /* 3002_success01 (used for pulleys) */
		WRITE_MEMORY(0x11D72DC, uint32_t, 0xFFFFFF); /* 3002_action06 */

		switch (Configuration::simulatorType)
		{
			case SimulatorType::Blue:
				ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SimulatorBlue", { "SimulatorSonic" }));
				ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SimulatorRocketBlue", { "PhantomRocket" }));
				ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SimulatorActionCommonBlue", { "SonicActionCommon" }));
				break;
			case SimulatorType::Black:
				ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SimulatorBlack", { "SimulatorSonic" }));
				ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SimulatorRocketBlack", { "PhantomRocket" }));
				ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SimulatorActionCommonBlack", { "SonicActionCommon" }));
				break;
			case SimulatorType::Green:
				ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SimulatorGreen", { "SimulatorSonic" }));
				ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SimulatorRocketGreen", { "PhantomRocket" }));
				ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SimulatorActionCommonGreen", { "SonicActionCommon" }));
				break;
			case SimulatorType::Pink:
				ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SimulatorPink", { "SimulatorSonic" }));
				ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SimulatorRocketPink", { "PhantomRocket" }));
				ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SimulatorActionCommonPink", { "SonicActionCommon" }));
				break;
			case SimulatorType::Red:
				ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SimulatorRed", { "SimulatorSonic" }));
				ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SimulatorRocketRed", { "PhantomRocket" }));
				ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SimulatorActionCommonRed", { "SonicActionCommon" }));
				break;
		}
	}

	ArchiveTreePatcher::applyPatches();
}