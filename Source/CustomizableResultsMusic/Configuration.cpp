#include "Configuration.h"

string Configuration::songChoice = "sound_sonic-Classic";
float Configuration::customDuration = 6.1f;
bool Configuration::customSingle = false;
SRank Configuration::SRankType = SRank::Default;

bool Configuration::load(const std::string& filePath)
{
#if _DEBUG
	printf("[Custom Results Music] Current loaded ini file: %s\n", filePath.c_str());
#endif
	const INIReader reader(filePath);
	if (reader.ParseError() != 0)
		return false;

	songChoice = reader.Get("Main", "IncludeDir0", songChoice);
	customDuration = reader.GetFloat("Config", "customDuration", customDuration);
	customSingle = reader.GetBoolean("Config", "customSingle", customSingle);
	SRankType = (SRank)reader.GetInteger("Config", "SRankType", SRankType);

	return true;
}
