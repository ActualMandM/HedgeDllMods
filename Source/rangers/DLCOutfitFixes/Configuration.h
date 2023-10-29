#pragma once

#include <INIReader.h>

class Configuration
{
	public:
		static bool incompatible;

		static bool sonicIncompatible;
		static bool sonicCyber;
		static bool sonicEffect;
		static bool sonicRealtime;
		static bool sonicJumpball;

		static bool friendsIncompatible;
		static bool friendsAmyMoves;
		static bool friendsTailsMoves;
		static bool friendsKnucklesMoves;

		static bool Load(const std::string& filePath);
};
