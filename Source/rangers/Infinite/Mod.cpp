#include "Configuration.h"
#include "CyloopManager.h"
#include "TextManager.h"

void manualConfig()
{
	MessageBox(nullptr, TEXT("Failed to load the config file!\nPlease configure the mod via Hedge Mod Manager.\n\nClosing."),
		TEXT("Infinite"), MB_ICONERROR);

	exit(-1);
}

SIG_SCAN(m_SonicVoice, "sound/voice_rangers_sonic", "?xxxxxxxxxxxxxxxxxxxxxxxx");

extern "C" __declspec(dllexport) void Init(ModInfo* modInfo)
{
	if (sigValid)
	{
		// Check if the configuration file exists
		if (!Configuration::load("infinite.ini"))
		{
			std::string executablePath;

			if (RegistryHelper::ReadString(HKEY_CURRENT_USER, "SOFTWARE\\HEDGEMM", "ExecutablePath", executablePath))
			{
				char sys[MAX_PATH * 2];
				{
					sprintf(sys, "\"\"%s\" --config \"%s\"\"", executablePath.c_str(), modInfo->CurrentMod->Path);
				}

				// Opens the HMM config dialog and waits for the process to exit.
				if (system(sys) == 0)
				{
					Configuration::load("infinite.ini");
				}
				else
				{
					manualConfig();
				}
			}
			else
			{
				manualConfig();
			}
		}

		if (Configuration::cyloop)
		{
			CyloopManager::init();
		}

		if (Configuration::text)
		{
			TextManager::init();
		}

		if (!Configuration::voice)
		{
			WRITE_MEMORY(m_SonicVoice(), uint8_t, 0);
		}
	}
	else
	{
		rangersVersionWarning(TEXT("Infinite"));
	}
}
