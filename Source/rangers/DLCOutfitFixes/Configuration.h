#pragma once

#include <INIReader.h>

class Configuration
{
	public:
		static bool sonicIncompatible;
		static bool sonicCyber;
		static bool sonicEffect;
		static bool sonicRealtime;
		static bool sonicJumpball;

		static bool amyIncompatible;
		static bool amyHammer;
		static bool amyTarot;

		static bool knucklesIncompatible;
		static bool knucklesDrill;

		static bool tailsIncompatible;
		static bool tailsCyblaster;
		static bool tailsCyclone;
		static bool tailsSpanner;
		static bool tailsSpring;

		static bool Load(const std::string& filePath);
};
