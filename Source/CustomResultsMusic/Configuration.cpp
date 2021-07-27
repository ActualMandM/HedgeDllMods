#include "Configuration.h"

string Configuration::songChoice = "sound_sonic-Classic";
float Configuration::customDuration = 6.1f;
bool Configuration::customSingle = false;

bool Configuration::load(const std::string& filePath)
{
	const INIReader reader(filePath);
	if (reader.ParseError() != 0)
		return false;

	songChoice = reader.Get("Main", "IncludeDir0", "sound_sonic-Classic");
	customDuration = reader.GetFloat("Config", "customDuration", 6.1f);
	customSingle = reader.GetBoolean("Config", "customSingle", false);

	return true;
}
