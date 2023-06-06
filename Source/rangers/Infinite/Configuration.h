#pragma once

#include <INIReader.h>

class Configuration
{
	public:
		static bool cyloop;
		static bool text;
		static bool voice;

		static bool load(const std::string& filePath);
};
