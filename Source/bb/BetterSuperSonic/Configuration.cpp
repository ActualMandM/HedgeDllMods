#include "Configuration.h"

bool Configuration::SuperSonicGoal = false;

bool Configuration::load(const std::string& filePath)
{
	const INIReader reader(filePath);

	if (reader.ParseError() != 0)
		return false;

	SuperSonicGoal = (bool)reader.GetInteger("Main", "SuperSonicGoal", false);

	return true;
}
