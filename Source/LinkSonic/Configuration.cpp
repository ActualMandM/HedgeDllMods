#include "Configuration.h"

TunicType Configuration::tunicType = TunicType::Green;
bool Configuration::slwEyes = false;

bool Configuration::load(const std::string& filePath)
{
	const INIReader reader(filePath);
	if (reader.ParseError() != 0)
		return false;

	tunicType = (TunicType)reader.GetInteger("Appearance", "TunicType", TunicType::Green);
	slwEyes = reader.GetBoolean("Appearance", "slwEyes", slwEyes);

	return true;
}
