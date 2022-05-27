#pragma once

enum Results
{
	Generations = -1,
	Custom = 0,
	Sonic1 = 1,
	SCDJP = 2,
	SCDUS = 3,
	S3K = 4,
	SonicR = 5,
	Adventure = 6,
	Adventure2 = 7,
	Shadow = 8,
	Sonic06 = 9,
	Sonic06Town = 10,
	SecretRings = 11,
	Unleashed = 12,
	SmashBros = 13,
	BlackKnight = 14,
	S4E1 = 15,
	Colors = 16,
	// ColorsBoss = 17,
	S4E2 = 18,
	LostWorld = 19,
	Mania = 20,
	Forces = 21,
	SRB2 = 22,
	Persona = 23,
	Persona2IS = 24,
	Persona2EP = 25,
	Persona3 = 26,
	Persona4 = 27,
	Persona5 = 28,
	Custom2 = 29,
	Rush = 30,
	RushAdventure = 31,
	ColorsSim = 32
};

enum SRank
{
	Default,
	Always,
	Never
};

class Configuration
{
	public:
		static Results ResultOptionClassic;
		static Results ResultOptionModern;
		static SRank SRankType;
		static bool OnlyRoundClear;
		static bool Crossfade;

		static bool CustomOnlyRC;
		static bool CustomBoss;
		static bool CustomERank;
		static float CustomDuration;

		static bool Custom2OnlyRC;
		static bool Custom2Boss;
		static bool Custom2ERank;
		static float Custom2Duration;

		static bool load(const std::string& filePath);
};
