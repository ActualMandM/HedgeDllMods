#include "Configuration.h"

Results Configuration::ResultOptionClassic = Results::Custom;
Results Configuration::ResultOptionModern = Results::Custom;
SRank Configuration::SRankType = SRank::Default;
bool Configuration::OnlyRoundClear = false;
bool Configuration::CustomOnlyRC = false;
float Configuration::CustomDuration = 6.1f;
bool Configuration::Custom2OnlyRC = false;
float Configuration::Custom2Duration = 6.1f;

bool Configuration::load(const std::string& filePath)
{
#if _DEBUG
	printf("[Custom Results Music] Loaded ini file: %s\n", filePath.c_str());
#endif
	const INIReader reader(filePath);
	if (reader.ParseError() != 0)
		return false;

	// Global Options
	ResultOptionClassic = (Results)reader.GetInteger("Config", "ResultOptionClassic", ResultOptionClassic);
	ResultOptionModern = (Results)reader.GetInteger("Config", "ResultOptionModern", ResultOptionModern);
	SRankType = (SRank)reader.GetInteger("Config", "SRankType", SRankType);
	OnlyRoundClear = reader.GetBoolean("Config", "OnlyRoundClear", OnlyRoundClear);
	// Custom Options
	CustomOnlyRC = reader.GetBoolean("Config", "CustomOnlyRC", CustomOnlyRC);
	CustomDuration = reader.GetFloat("Config", "customDuration", CustomDuration);
	Custom2OnlyRC = reader.GetBoolean("Config", "Custom2OnlyRC", Custom2OnlyRC);
	Custom2Duration = reader.GetFloat("Config", "custom2Duration", Custom2Duration);

	return true;
}