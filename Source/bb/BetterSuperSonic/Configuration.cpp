#include "Configuration.h"

bool Configuration::SuperSonicGoal = false;
bool Configuration::SuperSonicToggle = true;
bool Configuration::SkillOnly = false;

bool Configuration::load(const std::string& filePath)
{
	const INIReader reader(filePath);

	if (reader.ParseError() != 0)
		return false;

	SuperSonicGoal = (bool)reader.GetInteger("Main", "SuperSonicGoal", false);
	SuperSonicToggle = (bool)reader.GetInteger("Main", "SuperSonicToggle", true);
	SkillOnly = (bool)reader.GetInteger("Main", "SkillOnly", false);

	return true;
}
