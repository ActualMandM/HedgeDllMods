#include "Configuration.h"

bool Configuration::enableSimulator = false;
bool Configuration::enableGenerations = false;
SimulatorType Configuration::simulatorType = SimulatorType::Blue;
CostumeType Configuration::costumeType = CostumeType::None;

bool Configuration::load(const std::string& filePath)
{
	const INIReader reader(filePath);
	if (reader.ParseError() != 0)
		return false;

	enableSimulator = reader.GetBoolean("Appearance", "EnableSimulator", false);
	enableGenerations = reader.GetBoolean("Appearance", "EnableGenerations", false);
	simulatorType = (SimulatorType)reader.GetInteger("Appearance", "SimulatorType", SimulatorType::Blue);
	costumeType = (CostumeType)reader.GetInteger("Appearance", "CostumeType", CostumeType::None);

	return true;
}
