#include "Configuration.h"

ModelType Configuration::m_modelType = ModelType::Sonic;

bool Configuration::load(const std::string& filePath)
{
    const INIReader reader(filePath);
    if (reader.ParseError() != 0)
    {
        return false;
    }

    m_modelType = (ModelType)reader.GetInteger("Config", "nModel", 0);

    return true;
}
