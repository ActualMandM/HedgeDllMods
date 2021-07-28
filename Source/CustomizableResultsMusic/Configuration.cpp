#include "Configuration.h"

string Configuration::songChoice = "sound_sonic-Classic";
float Configuration::customDuration = 6.1f;
bool Configuration::customSingle = false;
bool Configuration::onlySRank = false;
bool Configuration::noSRank = false;

bool Configuration::load(const std::string& filePath)
{
	const INIReader reader(filePath);
	if (reader.ParseError() != 0)
		return false;

	songChoice = reader.Get("Main", "IncludeDir0", songChoice);
	customDuration = reader.GetFloat("Config", "customDuration", customDuration);
	customSingle = reader.GetBoolean("Config", "customSingle", customSingle);
	onlySRank = reader.GetBoolean("Config", "onlySRank", onlySRank);
	noSRank = reader.GetBoolean("Config", "noSRank", noSRank);

	return true;
}
