#include "Configuration.h"

GoalType Configuration::goalType = None;
bool Configuration::superSonicToggle = true;
bool Configuration::skillOnly = false;
bool Configuration::bpcSuper = true;

bool Configuration::load(const std::string& filePath)
{
	const INIReader reader(filePath);

	if (reader.ParseError() != 0)
		return false;

	goalType = (GoalType)reader.GetInteger("Main", "GoalType", None);
	superSonicToggle = (bool)reader.GetInteger("Main", "SuperSonicToggle", true);
	skillOnly = (bool)reader.GetInteger("Main", "SkillOnly", false);
	bpcSuper = (bool)reader.GetInteger("Main", "BPCSuper", true);

	return true;
}
