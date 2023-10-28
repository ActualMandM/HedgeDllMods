#include "Configuration.h"

bool Configuration::incompatible = false;

bool Configuration::sonicIncompatible = false;
bool Configuration::sonicCyber = false;
bool Configuration::sonicEffect = false;
bool Configuration::sonicRealtime = false;
bool Configuration::sonicJumpball = false;

bool Configuration::friendsIncompatible = false;

bool Configuration::Load(const std::string& filePath)
{
	const INIReader reader(filePath);

	if (reader.ParseError() != 0)
		return false;

	incompatible = reader.GetBoolean("Main", "Incompatible", false);

	sonicIncompatible = reader.GetBoolean("Sonic", "Incompatible", false);
	sonicCyber = reader.GetBoolean("Sonic", "Cyber", false);
	sonicEffect = reader.GetBoolean("Sonic", "Effect", false);
	sonicRealtime = reader.GetBoolean("Sonic", "Realtime", false);
	sonicJumpball = reader.GetBoolean("Sonic", "Jumpball", false);

	friendsIncompatible = reader.GetBoolean("Friends", "Incompatible", false);

	return true;
}
