#include "Configuration.h"

void SingleResults()
{
#if _DEBUG
	printf("[Custom Results Music] Round Clear will only play\n");
#endif
	WRITE_MEMORY(0xCFD47F, uint8_t, 0xE9, 0x95, 0x00, 0x00, 0x00, 0xC3, 0x90, 0x90, 0x90);
}

void ResultsDuration(float duration)
{
#if _DEBUG
	printf("[Custom Results Music] Set duration: %f\n", duration);
#endif
	// Special thanks to brianuuu for finding the address needed for adjusting the duration!
	WRITE_MEMORY(0x17046C0, double, duration);
}

void SRankResult(SRank SRankOption)
{
	if (SRankOption == Always)
	{
		WRITE_MEMORY(0xCFD4CA, uint8_t, 0xF8, 0x38);
		WRITE_MEMORY(0xCFD4E8, uint8_t, 0xF8, 0x38);
	}
	else if (SRankOption == Never)
	{
		WRITE_MEMORY(0xCFD4CA, uint8_t, 0x00, 0x39);
		WRITE_MEMORY(0xCFD4E8, uint8_t, 0x00, 0x39);
	}
}

extern "C" __declspec(dllexport) void Init()
{
	Configuration::load("mod.ini");

#if _DEBUG
	printf("[Custom Results Music] Current directory: %s\n", Configuration::songChoice.c_str());
#endif

	if (Configuration::songChoice == "sound_sonic-2006")
		ResultsDuration(7.381f);
	else if (Configuration::songChoice == "sound_sonic-Unleashed")
		ResultsDuration(6.021f);
	else if (Configuration::songChoice == "sound_sonic-Colors")
		ResultsDuration(8.01f);
	else if (Configuration::songChoice == "sound_sonic-LostWorld")
		ResultsDuration(8.182f);
	else if (Configuration::songChoice == "sound_sonic-BlackKnight")
		ResultsDuration(10.0f);
	else if (Configuration::songChoice != "" && Configuration::songChoice != "sound_Custom")
		SingleResults();

	// Not possible until I can modify two ini files via HMM ConfigSchema
	//Configuration::load("config.ini");

	if (Configuration::songChoice == "sound_Custom")
	{
		if (Configuration::customSingle)
			SingleResults();
		else
			ResultsDuration(Configuration::customDuration);
	}

	SRankResult(Configuration::SRankType);
}
