#include "Configuration.h"

bool Configuration::sonicIncompatible = false;
bool Configuration::sonicCyber = false;
bool Configuration::sonicEffect = false;
bool Configuration::sonicRealtime = false;
bool Configuration::sonicJumpball = false;

bool Configuration::amyIncompatible = false;
bool Configuration::amyHammer = false;
bool Configuration::amyTarot = false;

bool Configuration::knucklesIncompatible = false;
bool Configuration::knucklesDrill = false;

bool Configuration::tailsIncompatible = false;
bool Configuration::tailsCyblaster = false;
bool Configuration::tailsCyclone = false;
bool Configuration::tailsSpanner = false;
bool Configuration::tailsSpring = false;

bool Configuration::Load(const std::string& filePath)
{
	const INIReader reader(filePath);

	if (reader.ParseError() != 0)
		return false;

	sonicIncompatible = reader.GetBoolean("Sonic", "Incompatible", sonicIncompatible);
	sonicCyber = reader.GetBoolean("Sonic", "Cyber", sonicCyber);
	sonicEffect = reader.GetBoolean("Sonic", "Effect", sonicEffect);
	sonicRealtime = reader.GetBoolean("Sonic", "Realtime", sonicRealtime);
	sonicJumpball = reader.GetBoolean("Sonic", "Jumpball", sonicJumpball);

	amyIncompatible = reader.GetBoolean("Amy", "Incompatible", amyIncompatible);
	amyHammer = reader.GetBoolean("Amy", "Hammer", amyHammer);
	amyTarot = reader.GetBoolean("Amy", "Tarot", amyTarot);

	knucklesIncompatible = reader.GetBoolean("Knuckles", "Incompatible", knucklesIncompatible);
	knucklesDrill = reader.GetBoolean("Knuckles", "Drill", knucklesDrill);

	tailsIncompatible = reader.GetBoolean("Tails", "Incompatible", tailsIncompatible);
	tailsCyblaster = reader.GetBoolean("Tails", "Cyblaster", tailsCyblaster);
	tailsCyclone = reader.GetBoolean("Tails", "Cyclone", tailsCyclone);
	tailsSpanner = reader.GetBoolean("Tails", "Spanner", tailsSpanner);
	tailsSpring = reader.GetBoolean("Tails", "Spring", tailsSpring);

	return true;
}
