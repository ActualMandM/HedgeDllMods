#include "Configuration.h"

Results Configuration::ResultOptionClassic = Results::Custom;
Results Configuration::ResultOptionModern = Results::Custom;
SRank Configuration::SRankType = SRank::Default;
bool Configuration::OnlyRoundClear = false;
float Configuration::CustomDuration = 6.1f;

bool Configuration::load(const std::string& filePath)
{
	const INIReader reader(filePath);
	if (reader.ParseError() != 0)
		return false;

	ResultOptionClassic = (Results)reader.GetInteger("Config", "ResultOption", ResultOptionClassic);
	ResultOptionModern = (Results)reader.GetInteger("Config", "ResultOption", ResultOptionModern);
	SRankType = (SRank)reader.GetInteger("Config", "SRankType", SRankType);
	OnlyRoundClear = reader.GetBoolean("Config", "OnlyRoundClear", OnlyRoundClear);
	CustomDuration = reader.GetFloat("Config", "customDuration", CustomDuration);

	return true;
}
