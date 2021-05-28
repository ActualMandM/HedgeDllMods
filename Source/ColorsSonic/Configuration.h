#pragma once

enum SimulatorType
{
	Blue = 0,
	Black = 1,
	Green = 2,
	Pink = 3,
	Red = 4
};

enum CostumeType
{
	Default,
	Gold,
	Silver,
	Paint
};

class Configuration
{
	public:
		static bool enableSimulator;
		static SimulatorType simulatorType;
		static CostumeType costumeType;
	
		static bool load(const std::string& filePath);
};
