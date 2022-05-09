#pragma once

enum SonicType
{
	Colors,
	Simulator,
	Generations,
	ColorsUltimate
};

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
	Paint,
	PolkaDot
};

class Configuration
{
	public:
		static SonicType sonicType;
		static SimulatorType simulatorType;
		static CostumeType costumeType;
	
		static bool load(const std::string& filePath);
};
