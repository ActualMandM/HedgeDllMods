#pragma once

enum SRank
{
	Default,
	Always,
	Never
};

class Configuration
{
	public:
		static string songChoice;
		static float customDuration;
		static bool customSingle;
		static SRank SRankType;

		static bool load(const std::string& filePath);
};
