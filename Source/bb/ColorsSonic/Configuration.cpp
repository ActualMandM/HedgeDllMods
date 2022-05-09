#include "Configuration.h"

SonicType Configuration::sonicType = SonicType::Colors;
SimulatorType Configuration::simulatorType = SimulatorType::Blue;
CostumeType Configuration::costumeType = CostumeType::None;

bool Configuration::load(const std::string& filePath)
{
	const INIReader reader(filePath);
	if (reader.ParseError() != 0)
		return false;

	sonicType = (SonicType)reader.GetInteger("Appearance", "SonicType", SonicType::Colors);
	simulatorType = (SimulatorType)reader.GetInteger("Appearance", "SimulatorType", SimulatorType::Blue);
	costumeType = (CostumeType)reader.GetInteger("Appearance", "CostumeType", CostumeType::None);

	return true;
}
