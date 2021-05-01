#pragma once

enum ModelType
{
    Sonic = 0,
};

class Configuration
{
public:

    static ModelType m_modelType;

    static bool load(const std::string& filePath);
};
