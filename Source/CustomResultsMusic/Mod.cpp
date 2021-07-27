#include "Configuration.h"

void SingleResults()
{
	WRITE_MEMORY(0xCFD47F, uint8_t, 0xE9, 0x95, 0x00, 0x00, 0x00, 0xC3, 0x90, 0x90, 0x90);
}

void ResultsDuration(float duration)
{
	WRITE_MEMORY(0x17046C0, double, duration);
}

extern "C" __declspec(dllexport) void Init()
{
	if (Configuration::songChoice == "sound_Custom")
	{
		if (Configuration::customSingle)
			SingleResults();
		else
			ResultsDuration(Configuration::customDuration);
	}
	else if (Configuration::songChoice == "sound_sonic-Colors")
		ResultsDuration(8.01f);
	else if (Configuration::songChoice != "")
		SingleResults();
}