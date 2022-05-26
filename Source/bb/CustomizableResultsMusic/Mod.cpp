#include "Configuration.h"

struct resultData
{
	Results resultType; // The chosen results.
	double clearLength; // How long act clear lasts for (-1 for only clear).
	bool hasERank; // If the chosen results has a E-Rank variant.
	bool hasBoss; // If the chosen results has a boss clear variant.
};

const char* resultsChar[] =
{
	"Custom",
	"Sonic1",
	"SCDJP",
	"SCDUS",
	"S3K",
	"SonicR",
	"Adventure",
	"Adventure2",
	"Shadow",
	"Sonic06",
	"Sonic06Town",
	"SecretRings",
	"Unleashed",
	"SmashBros",
	"BlackKnight",
	"S4E1",
	"Colors",
	"ColorsBoss", // Unused (until repurposed)
	"S4E2",
	"LostWorld",
	"Mania",
	"Forces",
	"SRB2",
	"Persona",
	"Persona2IS",
	"Persona2EP",
	"Persona3",
	"Persona4",
	"Persona5",
	"Custom2",
	"Rush",
	"RushAdventure",
	"ColorsSim"
};

resultData modernResults;
resultData classicResults;

resultData currentResults;

char clearString[32];
char bossString[32];
char result0String[32];
char result1String[32];
char result2String[32];

// This prepares the result data.
resultData PrepareResults(bool isModern)
{
	Results resultType = Results::Generations;
	double clearLength = 6.099999904632568; // Default Generations length. (0x17046C0)
	bool hasERank = false;
	bool hasBoss = false;

	if (isModern)
		resultType = Configuration::ResultOptionModern;
	else
		resultType = Configuration::ResultOptionClassic;

	switch (resultType)
	{
		case Sonic06:
		case Sonic06Town:
		{
			clearLength = 7.381;
			break;
		}
		case Unleashed:
		{
			clearLength = 6.021;
			hasERank = true;
			hasBoss = true;
			break;
		}
		case Colors:
		case ColorsSim:
		{
			clearLength = 8.01;

			if (resultType != Results::ColorsSim)
				hasBoss = true;

			break;
		}
		case LostWorld:
		{
			clearLength = 8.182;
			break;
		}
		case BlackKnight:
		{
			clearLength = 10.0;
			hasERank = true;
			break;
		}
		case SmashBros:
		{
			clearLength = 7.631;
			break;
		}
		case Custom:
		{
			if (Configuration::CustomOnlyRC)
				clearLength = -1;
			else
				clearLength = (double)Configuration::CustomDuration;

			hasERank = Configuration::CustomERank;
			hasBoss = Configuration::CustomBoss;

			break;
		}
		case Custom2:
		{
			if (Configuration::Custom2OnlyRC)
				clearLength = -1;
			else
				clearLength = (double)Configuration::Custom2Duration;

			hasERank = Configuration::Custom2ERank;
			hasBoss = Configuration::Custom2Boss;

			break;
		}
		default:
		{
			if (resultType != Results::Generations)
				clearLength = -1;

			break;
		}
	}

	if (hasERank && !isModern)
		hasERank = false;

	return { resultType, clearLength, hasERank, hasBoss };
}

// This prepares the strings that will get used.
void PrepareStrings(resultData resultData)
{
	int resultType = (int)resultData.resultType;
	const char* resultsName = resultType < 0 ? "" : resultsChar[resultType];

	strcpy(clearString, resultType < 0 ? "Result" : std::format("RC_{}", resultsName).c_str());
	strcpy(bossString, resultType < 0 ? "Result" : std::format("RC_{}Boss", resultsName).c_str());

	strcpy(result0String, resultType < 0 ? "Result1" : std::format("R0_{}", resultsName).c_str());
	strcpy(result1String, resultType < 0 ? "Result1" : std::format("R1_{}", resultsName).c_str());
	strcpy(result2String, resultType < 0 ? "Result2" : std::format("R2_{}", resultsName).c_str());

	// S-Rank Type configuration
	if (Configuration::SRankType == Always)
		strcpy(result2String, resultType < 0 ? "Result1" : std::format("R1_{}", resultsName).c_str());
	else if (Configuration::SRankType == Never)
		strcpy(result1String, resultType < 0 ? "Result2" : std::format("R2_{}", resultsName).c_str());
}

/////////////////
// HOOKS BELOW //
/////////////////

// This fades out the previous track instead of making it stop.
int PlayMusicFadeOutPrevious(int a1, const Hedgehog::Base::CSharedString& name, float fadeInTime)
{
	int result = 0;
	uint32_t func = 0xD62C90;
	__asm
	{
		push fadeInTime
		push name
		mov eax, a1
		call func
		//add esp, 8

		mov result, eax
	}
	return result;
}

// This stops the previous track instead of making it fade out.
// FUNCTION_PTR(uint32_t, __stdcall, PlayResultMusic, 0xD62440, int a1, const Hedgehog::Base::CSharedString& name, float fadeInTime);

HOOK(void, __fastcall, _PlayActClear, 0xCFD2D0, int This)
{
	original_PlayActClear(This);

	uint32_t gameplayFlowStageAct = *reinterpret_cast<uint32_t*>(This + 8);
	uint32_t sender = *reinterpret_cast<uint32_t*>(gameplayFlowStageAct + 96);

	// There's a chance that this will have false positives. Find an alternative if this is the case.
	bool isModern = Sonic::Player::CSonicClassicContext::GetInstance() == nullptr;

	if (isModern)
		currentResults = modernResults;
	else
		currentResults = classicResults;

	PrepareStrings(currentResults);

	bool isBoss = currentResults.hasBoss && (Helpers::CheckCurrentStage("bms") || Helpers::CheckCurrentStage("bde") || Helpers::CheckCurrentStage("bsd") || Helpers::CheckCurrentStage("bpc") || Helpers::CheckCurrentStage("bsl") || Helpers::CheckCurrentStage("bne") || Helpers::CheckCurrentStage("blb"));

	PlayMusicFadeOutPrevious(sender, isBoss ? bossString : clearString, 0.0f);
}

// This function sets the time once, we don't need to nop it or anything.
HOOK(void*, __fastcall, _CalcScoreAndSetTime, 0xCFD550, int This)
{
	void* result = original_CalcScoreAndSetTime(This);
	*(double*)(This + 0x2C) = currentResults.clearLength - 0.25;
	return result;
}

HOOK(void, __fastcall, _PlayResults, 0xCFD410, int This)
{
	if (Configuration::OnlyRoundClear || currentResults.clearLength == -1)
		return;

	// Get AppDocument this way w/o dealing with "CHolderBase"
	auto appDocument = Sonic::CApplicationDocument::GetInstance();
	auto docMember = appDocument->m_pMember;

	// Awful pointer arithmetic to get the data we need.
#pragma region Constants
	const uint32_t gameplayFlowStageAct = *reinterpret_cast<uint32_t*>(This + 8);

	const uint32_t sender = *reinterpret_cast<uint32_t*>(gameplayFlowStageAct + 96);

	const uint32_t gameParameter = *reinterpret_cast<uint32_t*>(reinterpret_cast<uint32_t>(docMember) + 0x1B4);

	const float deltaTime = *reinterpret_cast<float*>(*reinterpret_cast<uint32_t*>(This + 0x0C) + 0x18);

	const unsigned __int8 compareFlag = (unsigned __int8)**reinterpret_cast<uint32_t**>(gameParameter + 0x80);

	// Signed, because it can be negative with Score Generations's E-Rank.
	const int32_t rank = *reinterpret_cast<int32_t*>(gameplayFlowStageAct + 0x174);
#pragma endregion 

	// Pointer data that we're going to modify.
	double* pSongTime = reinterpret_cast<double*>(This + 0x2C);
	uint8_t* pScoreType = reinterpret_cast<uint8_t*>(This + 0x28);

	///////////////////////
	// Logic starts here //
	///////////////////////

	if (!(compareFlag <= 0x1A && !*pScoreType))
		return;

	*pSongTime -= (double)deltaTime;

	if (*pSongTime >= 0.0)
		return;

	bool isSRank = rank == 4;
	bool isERank = rank < 0; // For Score Generations support.

	*pScoreType = isSRank ? 1 : 2;

	char resultMusic[32];

	if (isERank && currentResults.hasERank)
		strcpy(resultMusic, result0String);
	else
		strcpy(resultMusic, isSRank ? result2String : result1String);

	// Simply do what Generations does for now.
	PlayMusicFadeOutPrevious(sender, resultMusic, 0.0f);
}

extern "C" __declspec(dllexport) void Init()
{
	// Load the configuration file.
	if (!Configuration::load("CRM.ini"))
	{
		MessageBox(nullptr, TEXT("Failed to load CRM.ini!\nPlease configure the mod via HedgeModManager's Configure Mod option."),
			TEXT("Customizable Results Music"), MB_ICONERROR);

		exit(-1);
	}

	// Prepare the results data.
	modernResults = PrepareResults(true);
	classicResults = PrepareResults(false);

	// Install the hooks.
	WRITE_JUMP(0xCFD3C8, 0xCFD3D6);
	WRITE_JUMP(0xCFD3DD, 0xCFD400);
	INSTALL_HOOK(_PlayActClear);
	INSTALL_HOOK(_CalcScoreAndSetTime);
	INSTALL_HOOK(_PlayResults);
}
