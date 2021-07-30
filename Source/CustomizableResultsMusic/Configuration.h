#pragma once

enum Results
{
	Custom,
	Classic,
	SCDJP,
	SCDUS,
	S3K,
	SonicR,
	SA1,
	SA2,
	Shadow,
	S06,
	S06Town,
	SecretRings,
	Unleashed,
	UnleashedBoss,
	BlackKnight,
	S4E1,
	Colors,
	ColorsBoss,
	S4E2,
	SLW,
	Mania,
	Forces,
	SRB2,
	Persona1,
	Persona2IS,
	Persona2EP,
	Persona3,
	Persona4,
	Persona5
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
		static Results ResultOption;
		static SRank SRankType;
		static bool OnlyRoundClear;
		static float CustomDuration;

		static bool load(const std::string& filePath);
};
