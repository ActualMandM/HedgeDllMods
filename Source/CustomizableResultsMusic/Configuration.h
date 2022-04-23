#pragma once

enum Results
{
	Custom,
	Sonic1,
	SCDJP,
	SCDUS,
	S3K,
	SonicR,
	Adventure,
	Adventure2,
	Shadow,
	Sonic06,
	Sonic06Town,
	SecretRings,
	Unleashed,
	UnleashedBoss,
	BlackKnight,
	S4E1,
	Colors,
	ColorsBoss,
	S4E2,
	LostWorld,
	Mania,
	Forces,
	SRB2,
	Persona,
	Persona2IS,
	Persona2EP,
	Persona3,
	Persona4,
	Persona5,
	Custom2,
	Rush,
	RushAdventure,
	ColorsSim
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
		static bool CustomOnlyRC;
		static float CustomDuration;
		static bool Custom2OnlyRC;
		static float Custom2Duration;

		static bool load(const std::string& filePath);
};
