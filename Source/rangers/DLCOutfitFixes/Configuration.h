#pragma once

#include <INIReader.h>

class Configuration
{
	public:
		static bool cyber;
		static bool effect;
		static bool realtime;
		static bool jumpball;

		static bool Load(const std::string& filePath);
};
