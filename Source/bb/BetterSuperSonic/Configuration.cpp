#include "Configuration.h"

SonicCheck Configuration::goalType = None;
SonicCheck Configuration::superMusic = None;
bool Configuration::saveCheck = true;
bool Configuration::superSonicToggle = true;
bool Configuration::skillOnly = false;
bool Configuration::bpcSuper = true;

bool Configuration::load(const std::string& filePath)
{
	const INIReader reader(filePath);

	if (reader.ParseError() != 0)
		return false;

	goalType = (SonicCheck)reader.GetInteger("Main", "GoalType", None);
	superMusic = (SonicCheck)reader.GetInteger("Main", "SuperMusic", None);
	saveCheck = (bool)reader.GetInteger("Main", "SaveCheck", true);
	superSonicToggle = (bool)reader.GetInteger("Main", "SuperSonicToggle", true);
	skillOnly = (bool)reader.GetInteger("Main", "SkillOnly", false);
	bpcSuper = (bool)reader.GetInteger("Main", "BPCSuper", true);

	return true;
}
