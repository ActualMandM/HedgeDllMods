#include "Configuration.h"

TunicType Configuration::tunicType = TunicType::Green;

bool Configuration::load(const std::string& filePath)
{
	const INIReader reader(filePath);
	if (reader.ParseError() != 0)
	{
		return false;
	}

	tunicType = (TunicType)reader.GetInteger("Appearance", "TunicType", 0);

	return true;
}
