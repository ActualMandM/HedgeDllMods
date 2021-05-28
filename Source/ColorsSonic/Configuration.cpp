#include "Configuration.h"

bool Configuration::enableSimulator = false;
SimulatorType Configuration::simulatorType = SimulatorType::Blue;
CostumeType Configuration::costumeType = CostumeType::Default;

bool Configuration::load(const std::string& filePath)
{
	const INIReader reader(filePath);
	if (reader.ParseError() != 0)
		return false;

	enableSimulator = reader.GetBoolean("Appearance", "EnableSimulator", false);
	simulatorType = (SimulatorType)reader.GetInteger("Appearance", "SimulatorType", SimulatorType::Blue);
	costumeType = (CostumeType)reader.GetInteger("Appearance", "CostumeType", CostumeType::Default);

	return true;
}
