#include "ArchiveTreePatcher.h"
#include "Configuration.h"

vector<string> sonicArchives = { "Sonic", "Title", "ev031", "ev041", "ev042", "ev091", "evSonic" };
static const char* costumeType[] = { "", "MetalGold", "MetalSilver", "Paint", "PolkaDot" };
static const char* simColors[] = { "Blue", "Black", "Green", "Pink", "Red" };

extern "C" __declspec(dllexport) void Init()
{
	// Disable Title Loading Video by HyperBE32
	WRITE_MEMORY(0xD6966E, uint8_t, 0xE9, 0x14, 0x01, 0x00, 0x00);

	// check if the configuration file exists
	if (!Configuration::load("ColorsSonic.ini"))
	{
		MessageBox(nullptr, TEXT("Failed to load the config file!\nPlease make sure that ColorsSonic.ini exists in the mod's folder."),
			TEXT("Colors Sonic"), MB_ICONERROR);
	}

	// CostumeType configuration
	if (Configuration::costumeType != None)
	{
		ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("Ultimate"
			+ std::string(costumeType[Configuration::costumeType]), { sonicArchives }));
	}

	// SimulatorType configuration
	if (Configuration::enableSimulator)
	{
		ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SimulatorSonic", { sonicArchives }));

		// Simulator Animation Swap by HyperBE32
		WRITE_MEMORY(0x15E6848, const char, "sim_start_wait_a\0");
		WRITE_MEMORY(0x15E6859, const char, "sim_start_normal\0");
		WRITE_MEMORY(0x1277C75, uint32_t, 0x015E6848);
		WRITE_MEMORY(0x1277C0E, uint32_t, 0x015E6859);

		// Disable Character Action Voices by HyperBE32
		WRITE_MEMORY(0xDF0B96, uint32_t, 0xFFFFFF);  /* 3002_action01 (used for Unleashed jump selector) */
		WRITE_MEMORY(0xDF0C08, uint32_t, 0xFFFFFF);  /* 3002_action01 (used for Unleashed jump selector) */
		WRITE_MEMORY(0xDFC993, uint32_t, 0xFFFFFF);  /* 3002_action01 (used for bouncing off physics objects) */
		WRITE_MEMORY(0xE4B684, uint32_t, 0xFFFFFF);  /* 3002_action01 (used for tricks) */
		WRITE_MEMORY(0xE4B8E0, uint32_t, 0xFFFFFF);  /* 3002_action01 (used for trick finish) */
		WRITE_MEMORY(0xE57E6F, uint32_t, 0xFFFFFF);  /* 3002_action01 (used for jumping) */
		WRITE_MEMORY(0x11D7B8C, uint32_t, 0xFFFFFF); /* 3002_action01 (used for grinding) */
		WRITE_MEMORY(0x123B475, uint32_t, 0xFFFFFF); /* 3002_action01 (used for cannons) */
		WRITE_MEMORY(0x1252740, uint32_t, 0xFFFFFF); /* 3002_action01 (used for squat kick) */
		WRITE_MEMORY(0x1117EF4, uint32_t, 0xFFFFFF); /* 3002_stumble */
		WRITE_MEMORY(0xE0248C, uint32_t, 0xFFFFFF);  /* 3002_damage01 */
		WRITE_MEMORY(0xE02496, uint32_t, 0xFFFFFF);  /* 3002_damage02 */
		WRITE_MEMORY(0xE024A0, uint32_t, 0xFFFFFF);  /* 3002_damage03 */
		WRITE_MEMORY(0xE024AA, uint32_t, 0xFFFFFF);  /* 3002_damage04 */
		WRITE_MEMORY(0xDF0B63, uint32_t, 0xFFFFFF);  /* 3002_damage05 (used for Unleashed jump selector) */
		WRITE_MEMORY(0xE4466E, uint32_t, 0xFFFFFF);  /* 3002_strain (used for pushing) */
		WRITE_MEMORY(0xE023E3, uint32_t, 0xFFFFFF);  /* 3002_dead (used for dying from enemies) */
		WRITE_MEMORY(0xE55D3D, uint32_t, 0xFFFFFF);  /* 3002_dead (used for dying from falling) */
		WRITE_MEMORY(0x1117CEB, uint32_t, 0xFFFFFF); /* 3002_action03 (used for boost) */
		WRITE_MEMORY(0x1118512, uint32_t, 0xFFFFFF); /* 3002_action03 (used for drift) */
		WRITE_MEMORY(0x1254E04, uint32_t, 0xFFFFFF); /* 3002_action03 (used for stomping) */
		WRITE_MEMORY(0xDF3710, uint32_t, 0xFFFFFF);  /* 3002_action04 (used for homing success) */
		WRITE_MEMORY(0x1117D09, uint32_t, 0xFFFFFF); /* 3002_action04 (used for boost) */
		WRITE_MEMORY(0x11184E4, uint32_t, 0xFFFFFF); /* 3002_action04 (used for homing success) */
		WRITE_MEMORY(0x1119B1F, uint32_t, 0xFFFFFF); /* 3002_action04 (used for board drift) */
		WRITE_MEMORY(0xE3DADD, uint32_t, 0xFFFFFF);  /* 3002_success01 (used for blast off) */
		WRITE_MEMORY(0xE462AA, uint32_t, 0xFFFFFF);  /* 3002_success01 (used for pulleys) */
		WRITE_MEMORY(0x1117D27, uint32_t, 0xFFFFFF); /* 3002_action05 (used for boost) */
		WRITE_MEMORY(0x1231971, uint32_t, 0xFFFFFF); /* 3002_action05 (used for light dash) */
		WRITE_MEMORY(0xDF0B63, uint32_t, 0xFFFFFF);  /* 3002_damage06 */
		WRITE_MEMORY(0x11D72DC, uint32_t, 0xFFFFFF); /* 3002_action06 */
		
		ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("Simulator"
			+ std::string(simColors[Configuration::simulatorType]), { "SimulatorSonic" }));

		ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SimulatorRocket"
			+ std::string(simColors[Configuration::simulatorType]), { "PhantomRocket" }));

		if (Configuration::simulatorType != Blue)
		{
			ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("SimulatorActionCommon"
				+ std::string(simColors[Configuration::simulatorType]), { "SonicActionCommon" }));
		}
	}
	// Inject Colors Sonic into event archives and title screen
	else if (!Configuration::enableGenerations)
		ArchiveTreePatcher::m_archiveDependencies.push_back(ArchiveDependency("ColorsSonic", { sonicArchives }));

	ArchiveTreePatcher::applyPatches();
}
