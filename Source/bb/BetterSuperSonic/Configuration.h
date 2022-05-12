#pragma once

class Configuration
{
	public:
		static bool SuperSonicGoal;
		static bool SuperSonicToggle;
		static bool SkillOnly;
	
		static bool load(const std::string& filePath);
};
