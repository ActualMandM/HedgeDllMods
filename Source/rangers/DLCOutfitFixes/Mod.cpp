#include "Mod.h"
#include "Configuration.h"

SIG_SCAN
(
	m_SigGetCurrentOutfit,
	"\x0F\xBE\xC1\xFF\xC8\x83\xF8\x05\x77\x24\x48\x8D\x15\x00\x00\x00\x00\x48\x98\x8B\x8C\x82\x00\x00\x00\x00\x48\x03\xCA\xFF\xE1\xB0\x01\xC3\xB0\x02\xC3\xB0\x04\xC3\xB0\x07\xC3\xB0\x08\xC3",
	"xxxxxxxxxxxxx????xxxxx????xxxxxxxxxxxxxxxxxxxx"
);

uint8_t outfit = 0;

HOOK(uint8_t, __fastcall, GetCurrentOutfit, m_SigGetCurrentOutfit(), uint8_t in_saveOutfitIdx)
{
	outfit = in_saveOutfitIdx;
	return originalGetCurrentOutfit(in_saveOutfitIdx);
}

SIG_SCAN
(
	m_SigSonicAuraVisibility,
	"\x3C\x02\x74\x16\x3C\x08\x74\x12\xB2\x01\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x89\x00\x40",
	"xxxxxxxxxxxxx????x????xx?x"
);

auto m_LoadResModel = READ_CALL(m_SigSonicAuraVisibility() - 0x1A);

HOOK(int64_t, __fastcall, LoadResModel, m_LoadResModel, const char* in_modelName, int64_t a2)
{
	char modelName[256];
	strcpy(modelName, in_modelName);

	if ((outfit > 0 && outfit <= MAX_OUTFIT) && (StringHelper::ContainsSubstring(modelName, "chr_") && StringHelper::ContainsSubstring(modelName, "sonic")))
	{
		uint8_t modelIdx = outfit - 1;

		if (!strcmp(in_modelName, "chr_sonic"))
			GetModelName(modelName, modelIdx, "sonic", nullptr);

		else if (!strcmp(in_modelName, "chr_sonicT"))
			GetModelName(modelName, modelIdx, "sonic", nullptr);

		else if (!strcmp(in_modelName, "chr_sonic_aura"))
			GetModelName(modelName, modelIdx, "sonic", "aura");

		else if (!strcmp(in_modelName, "chr_sonic_shape"))
			GetModelName(modelName, modelIdx, "sonic", "shape");

		else if (!strcmp(in_modelName, "chr_supersonic"))
			GetModelName(modelName, modelIdx, "supersonic", nullptr);

		else if (!strcmp(in_modelName, "chr_supersonic_aura"))
			GetModelName(modelName, modelIdx, "supersonic", "aura");

		else if (!strcmp(in_modelName, "chr_supersonic_shape"))
			GetModelName(modelName, modelIdx, "supersonic", "shape");

		else if (!strcmp(in_modelName, "chr_supersonic2"))
			GetModelName(modelName, modelIdx, "supersonic2", nullptr);

		// Can be toggled via configuration file
		else
		{
			if (Configuration::cyber && !strcmp(in_modelName, "chr_soniccyber"))
				GetModelName(modelName, modelIdx, "soniccyber", nullptr);

			if (Configuration::effect)
			{
				if (!strcmp(in_modelName, "chr_supersonic_kick_L"))
					GetModelName(modelName, modelIdx, "supersonic", "kick_L");

				else if (!strcmp(in_modelName, "chr_supersonic_kick_R"))
					GetModelName(modelName, modelIdx, "supersonic", "kick_R");

				else if (!strcmp(in_modelName, "chr_supersonic_punch_L"))
					GetModelName(modelName, modelIdx, "supersonic", "punch_L");

				else if (!strcmp(in_modelName, "chr_supersonic_punch_R"))
					GetModelName(modelName, modelIdx, "supersonic", "punch_R");
			}

			if (Configuration::realtime)
			{
				if (!strcmp(in_modelName, "chr_supersoniccyber"))
					GetModelName(modelName, modelIdx, "supersoniccyber", nullptr);

				else if (!strcmp(in_modelName, "chr_supersonicdamage"))
					GetModelName(modelName, modelIdx, "supersonicdamage", nullptr);
			}

			// NOTE: There's chr_supersonicspin too, but it only shows up with Homing Shot and Homing Attack
			if (Configuration::jumpball && !strcmp(in_modelName, "chr_sonicspin"))
				GetModelName(modelName, modelIdx, "sonicspin", nullptr);
		}
	}

	return originalLoadResModel(modelName, a2);
}

extern "C" __declspec(dllexport) void Init()
{
	if (sigValid)
	{
		// Check if the configuration file exists
		if (!Configuration::Load("config.ini"))
			printf("[Sonic Outfit Fixes] Config file failed to load!\n");

		// Disable aura visibility being removed for certain outfits
		WRITE_NOP(m_SigSonicAuraVisibility(), 8);
		INSTALL_HOOK(GetCurrentOutfit);

		printf("[Sonic Outfit Fixes] m_LoadResModel: 0x%llx\n", m_LoadResModel);
		INSTALL_HOOK(LoadResModel);
	}
	else
	{
		rangersVersionWarning(TEXT("Sonic Outfit Fixes"));
	}
}

extern "C" __declspec(dllexport) void PostInit(ModInfo* mods)
{
	if (sigValid)
	{
		// Check for configuration in loaded mods
		for (Mod* mod : *mods->ModList)
		{
			std::string configPath = StringHelper::GetSubstringBeforeLastChar(mod->Path, '\\').append("\\OutfitFixes.ini");

			if (Configuration::Load(configPath))
			{
				printf("[Sonic Outfit Fixes] Configuration was overridden by %s\n", mod->Name);
				break;
			}
		}
	}
}
