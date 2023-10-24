#include "Configuration.h"

bool Configuration::cyber = false;
bool Configuration::effect = false;
bool Configuration::realtime = false;
bool Configuration::jumpball = false;

bool Configuration::load(const std::string& filePath)
{
	const INIReader reader(filePath);

	if (reader.ParseError() != 0)
		return false;

	cyber = reader.GetBoolean("Main", "Cyber", false);
	effect = reader.GetBoolean("Main", "Effect", false);
	realtime = reader.GetBoolean("Main", "Realtime", false);
	jumpball = reader.GetBoolean("Main", "Jumpball", false);

	return true;
}
