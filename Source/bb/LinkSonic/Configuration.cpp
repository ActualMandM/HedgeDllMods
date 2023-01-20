#include "Configuration.h"

std::string Configuration::tunicType = "Green";

bool Configuration::load(const std::string& filePath)
{
	const INIReader reader(filePath);

	if (reader.ParseError() != 0)
		return false;

	tunicType = reader.Get("Appearance", "TunicType", "Green");

	return true;
}
