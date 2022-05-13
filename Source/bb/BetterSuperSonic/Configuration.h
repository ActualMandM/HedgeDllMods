#pragma once

enum GoalType
{
	None,
	Classic,
	Modern,
	Both
};

class Configuration
{
	public:
		static GoalType goalType;
		static bool superSonicToggle;
		static bool skillOnly;
		static bool bpcSuper;
	
		static bool load(const std::string& filePath);
};
