#include "Configuration.h"

bool Configuration::cyloop = true;
bool Configuration::text = true;
bool Configuration::voice = false;

bool Configuration::load(const std::string& filePath)
{
	const INIReader reader(filePath);

	if (reader.ParseError() != 0)
		return false;

	cyloop = reader.GetBoolean("Main", "Cyloop", true);
	text = reader.GetBoolean("Main", "Text", true);
	voice = reader.GetBoolean("Main", "Voice", false);

	return true;
}
