#include "Configuration.h"

Results Configuration::ResultOption = Results::Custom;
SRank Configuration::SRankType = SRank::Default;
bool Configuration::OnlyRoundClear = false;
float Configuration::CustomDuration = 6.1f;

bool Configuration::load(const std::string& filePath)
{
#if _DEBUG
	printf("[Custom Results Music] Current loaded ini file: %s\n", filePath.c_str());
#endif
	const INIReader reader(filePath);
	if (reader.ParseError() != 0)
		return false;

	ResultOption = (Results)reader.GetInteger("Config", "ResultOption", ResultOption);
	SRankType = (SRank)reader.GetInteger("Config", "SRankType", SRankType);
	OnlyRoundClear = reader.GetBoolean("Config", "OnlyRoundClear", OnlyRoundClear);
	CustomDuration = reader.GetFloat("Config", "customDuration", CustomDuration);

	return true;
}
