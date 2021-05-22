#pragma once

enum TunicType
{
	Green = 0,
	Red = 1,
	Blue = 2,
	Purple = 3
};

class Configuration
{
public:

	static TunicType tunicType;

	static bool load(const std::string& filePath);
};
