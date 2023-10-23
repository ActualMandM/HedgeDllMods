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
	if ((outfit > 0 && outfit <= MAX_OUTFIT) && StringHelper::ContainsSubstring(in_modelName, "chr_"))
	{
		uint8_t modelIdx = outfit - 1;

		if (!strcmp(in_modelName, "chr_sonicT"))
			in_modelName = GetModelName(modelIdx, "sonic", nullptr);

		if (!strcmp(in_modelName, "chr_sonic_aura"))
			in_modelName = GetModelName(modelIdx, "sonic", "aura");

		if (!strcmp(in_modelName, "chr_sonic_shape"))
			in_modelName = GetModelName(modelIdx, "sonic", "shape");

		if (!strcmp(in_modelName, "chr_supersonic"))
			in_modelName = GetModelName(modelIdx, "supersonic", nullptr);

		if (!strcmp(in_modelName, "chr_supersonic_aura"))
			in_modelName = GetModelName(modelIdx, "supersonic", "aura");

		if (!strcmp(in_modelName, "chr_supersonic_shape"))
			in_modelName = GetModelName(modelIdx, "supersonic", "shape");

		if (!strcmp(in_modelName, "chr_supersonic2"))
			in_modelName = GetModelName(modelIdx, "supersonic2", nullptr);

		// Can be toggled via configuration file
		{
			if (Configuration::cyber && !strcmp(in_modelName, "chr_soniccyber"))
				in_modelName = GetModelName(modelIdx, "soniccyber", nullptr);

			if (Configuration::effect)
			{
				if (!strcmp(in_modelName, "chr_supersonic_kick_L"))
					in_modelName = GetModelName(modelIdx, "supersonic", "kick_L");

				if (!strcmp(in_modelName, "chr_supersonic_kick_R"))
					in_modelName = GetModelName(modelIdx, "supersonic", "kick_R");

				if (!strcmp(in_modelName, "chr_supersonic_punch_L"))
					in_modelName = GetModelName(modelIdx, "supersonic", "punch_L");

				if (!strcmp(in_modelName, "chr_supersonic_punch_R"))
					in_modelName = GetModelName(modelIdx, "supersonic", "punch_R");
			}

			if (Configuration::realtime)
			{
				if (!strcmp(in_modelName, "chr_supersoniccyber"))
					in_modelName = GetModelName(modelIdx, "supersoniccyber", nullptr);

				if (!strcmp(in_modelName, "chr_supersonicdamage"))
					in_modelName = GetModelName(modelIdx, "supersonicdamage", nullptr);
			}
		}
	}

	return originalLoadResModel(in_modelName, a2);
}

extern "C" __declspec(dllexport) void Init()
{
	if (sigValid)
	{
		// Check if the configuration file exists
		if (!Configuration::load("config.ini"))
			printf("[Sonic Outfit Fixes] Config file failed to load!\n");

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
