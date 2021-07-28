#pragma once

class Configuration
{
	public:
		static string songChoice;
		static float customDuration;
		static bool customSingle;
		static bool onlySRank;
		static bool noSRank;

		static bool load(const std::string& filePath);
};
