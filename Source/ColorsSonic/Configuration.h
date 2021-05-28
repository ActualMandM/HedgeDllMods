#pragma once

enum SimulatorType
{
	Blue,
	Black,
	Green,
	Pink,
	Red
};

enum CostumeType
{
	None,
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
