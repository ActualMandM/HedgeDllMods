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

	incompatible = reader.GetBoolean("Main", "Incompatible", incompatible);

	sonicIncompatible = reader.GetBoolean("Sonic", "Incompatible", sonicIncompatible);
	sonicCyber = reader.GetBoolean("Sonic", "Cyber", sonicCyber);
	sonicEffect = reader.GetBoolean("Sonic", "Effect", sonicEffect);
	sonicRealtime = reader.GetBoolean("Sonic", "Realtime", sonicRealtime);
	sonicJumpball = reader.GetBoolean("Sonic", "Jumpball", sonicJumpball);

	friendsIncompatible = reader.GetBoolean("Friends", "Incompatible", friendsIncompatible);

	return true;
}
