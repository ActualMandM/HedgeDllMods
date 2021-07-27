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
	// YandereDev moment (not by choice)
	if (Configuration::songChoice == "sound_Custom")
	{
		if (Configuration::customSingle)
			SingleResults();
		else
			ResultsDuration(Configuration::customDuration);
	}
	else if (Configuration::songChoice == "sound_sonic-Classic"
		  || Configuration::songChoice == "sound_sonic-Knuckles"
		  || Configuration::songChoice == "sound_sonic-Adventure"
		  || Configuration::songChoice == "sound_sonic-Adventure2"
		  || Configuration::songChoice == "sound_sonic-SecretRings"
		  || Configuration::songChoice == "sound_sonic-4Episode1"
		  || Configuration::songChoice == "sound_sonic-4Episode2"
		  || Configuration::songChoice == "sound_sonic-Forces"
		  || Configuration::songChoice == ".")
		SingleResults();
	else if (Configuration::songChoice == "sound_sonic-Colors")
		ResultsDuration(8.01f);
}