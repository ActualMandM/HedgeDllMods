#pragma once

#include <INIReader.h>

class Configuration
{
	public:
		static bool incompatible;

		static bool sonicCyber;
		static bool sonicEffect;
		static bool sonicRealtime;
		static bool sonicJumpball;

		static bool Load(const std::string& filePath);
};
