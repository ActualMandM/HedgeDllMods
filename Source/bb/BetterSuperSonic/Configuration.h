#pragma once

enum SonicCheck
{
	None,
	Classic,
	Modern,
	Both
};

class Configuration
{
	public:
		static SonicCheck goalType;
		static SonicCheck superMusic;
		static bool saveCheck;
		static bool superSonicToggle;
		static bool skillOnly;
		static bool bpcSuper;
	
		static bool load(const std::string& filePath);
};
