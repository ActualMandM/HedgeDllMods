#pragma once

class Configuration
{
	public:
		static std::string tunicType;

		static bool load(const std::string& filePath);
};
