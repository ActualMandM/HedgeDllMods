#include "Configuration.h"

Results Configuration::ResultOptionClassic = Results::Custom;
Results Configuration::ResultOptionModern = Results::Custom;
SRank Configuration::SRankType = SRank::Default;
bool Configuration::OnlyRoundClear = false;

bool Configuration::CustomOnlyRC = false;
bool Configuration::CustomBoss = false;
bool Configuration::CustomERank = false;
float Configuration::CustomDuration = 6.1f;

bool Configuration::Custom2OnlyRC = false;
bool Configuration::Custom2Boss = false;
bool Configuration::Custom2ERank = false;
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
	ResultOptionClassic = (Results)reader.GetInteger("Config", "ResultOptionClassic", Results::Custom);
	ResultOptionModern = (Results)reader.GetInteger("Config", "ResultOptionModern", Results::Custom);
	SRankType = (SRank)reader.GetInteger("Config", "SRankType", SRank::Default);
	OnlyRoundClear = reader.GetBoolean("Config", "OnlyRoundClear", false);

	// Custom #1 Options
	CustomOnlyRC = reader.GetBoolean("Config", "CustomOnlyRC", false);
	CustomOnlyRC = reader.GetBoolean("Config", "CustomBoss", false);
	CustomOnlyRC = reader.GetBoolean("Config", "CustomERank", false);
	CustomDuration = reader.GetFloat("Config", "customDuration", 6.1f);

	// Custom #2 Options
	Custom2OnlyRC = reader.GetBoolean("Config", "Custom2OnlyRC", false);
	Custom2OnlyRC = reader.GetBoolean("Config", "Custom2Boss", false);
	Custom2OnlyRC = reader.GetBoolean("Config", "Custom2ERank", false);
	Custom2Duration = reader.GetFloat("Config", "custom2Duration", 6.1f);

	return true;
}
