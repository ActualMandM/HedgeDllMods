#pragma once

enum TunicType
{
	Green,
	Red,
	Blue,
	Purple,
	Rainbow,
	Slash,
	Bash,
	Foof
};

class Configuration
{
	public:
		static TunicType tunicType;

		static bool slwEyes;
	
		static bool load(const std::string& filePath);
};
