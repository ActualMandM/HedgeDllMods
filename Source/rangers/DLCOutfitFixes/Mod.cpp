#include "Mod.h"
#include "Configuration.h"

SIG_SCAN
(
	m_SigGetCurrentSonicOutfit,
	"\x0F\xBE\xC1\xFF\xC8\x83\xF8\x05\x77\x24\x48\x8D\x15\x00\x00\x00\x00\x48\x98\x8B\x8C\x82\x00\x00\x00\x00\x48\x03\xCA\xFF\xE1\xB0\x01\xC3\xB0\x02\xC3\xB0\x04\xC3\xB0\x07\xC3\xB0\x08\xC3",
	"xxxxxxxxxxxxx????xxxxx????xxxxxxxxxxxxxxxxxxxx"
);

uint8_t outfit = 0;

HOOK(uint8_t, __fastcall, GetCurrentSonicOutfit, m_SigGetCurrentSonicOutfit(), uint8_t in_saveOutfitIdx)
{
	outfit = in_saveOutfitIdx;
	return originalGetCurrentSonicOutfit(in_saveOutfitIdx);
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

	if (!Configuration::incompatible && StringHelper::ContainsSubstring(modelName, "chr_"))
	{
		// Sonic
		if ((outfit > 0 && outfit <= MAX_OUTFIT) && StringHelper::ContainsSubstring(modelName, "sonic"))
		{
			uint8_t modelIdx = outfit - 1;

			if (StringHelper::Compare(in_modelName, "chr_sonic"))
				GetSonicName(modelName, modelIdx, "sonic", nullptr);

			else if (StringHelper::Compare(in_modelName, "chr_sonicT"))
				GetSonicName(modelName, modelIdx, "sonic", nullptr);

			else if (StringHelper::Compare(in_modelName, "chr_sonic_aura"))
				GetSonicName(modelName, modelIdx, "sonic", "aura");

			else if (StringHelper::Compare(in_modelName, "chr_sonic_shape"))
				GetSonicName(modelName, modelIdx, "sonic", "shape");

			else if (StringHelper::Compare(in_modelName, "chr_supersonic"))
				GetSonicName(modelName, modelIdx, "supersonic", nullptr);

			else if (StringHelper::Compare(in_modelName, "chr_supersonic_aura"))
				GetSonicName(modelName, modelIdx, "supersonic", "aura");

			else if (StringHelper::Compare(in_modelName, "chr_supersonic_shape"))
				GetSonicName(modelName, modelIdx, "supersonic", "shape");

			else if (StringHelper::Compare(in_modelName, "chr_supersonic2"))
				GetSonicName(modelName, modelIdx, "supersonic2", nullptr);

			// Character mods can toggle these on via config
			else
			{
				if (Configuration::sonicCyber && StringHelper::Compare(in_modelName, "chr_soniccyber"))
					GetSonicName(modelName, modelIdx, "soniccyber", nullptr);

				if (Configuration::sonicEffect)
				{
					if (StringHelper::Compare(in_modelName, "chr_supersonic_kick_L"))
						GetSonicName(modelName, modelIdx, "supersonic", "kick_L");

					else if (StringHelper::Compare(in_modelName, "chr_supersonic_kick_R"))
						GetSonicName(modelName, modelIdx, "supersonic", "kick_R");

					else if (StringHelper::Compare(in_modelName, "chr_supersonic_punch_L"))
						GetSonicName(modelName, modelIdx, "supersonic", "punch_L");

					else if (StringHelper::Compare(in_modelName, "chr_supersonic_punch_R"))
						GetSonicName(modelName, modelIdx, "supersonic", "punch_R");
				}

				if (Configuration::sonicRealtime)
				{
					if (StringHelper::Compare(in_modelName, "chr_supersoniccyber"))
						GetSonicName(modelName, modelIdx, "supersoniccyber", nullptr);

					else if (StringHelper::Compare(in_modelName, "chr_supersonicdamage"))
						GetSonicName(modelName, modelIdx, "supersonicdamage", nullptr);
				}

				// NOTE: There's chr_supersonicspin too, but it only shows up with Homing Shot and Homing Attack
				if (Configuration::sonicJumpball && StringHelper::Compare(in_modelName, "chr_sonicspin"))
					GetSonicName(modelName, modelIdx, "sonicspin", nullptr);
			}
		}

		// Friends
		if (false)
		{
			if (StringHelper::Compare(in_modelName, "chr_amyP"))
				GetFriendName(modelName, "amyP", nullptr);

			else if (StringHelper::Compare(in_modelName, "chr_knucklesP"))
				GetFriendName(modelName, "knucklesP", nullptr);

			else if (StringHelper::Compare(in_modelName, "chr_tailsP"))
				GetFriendName(modelName, "tailsP", nullptr);
		}
	}

	return originalLoadResModel(modelName, a2);
}

extern "C" __declspec(dllexport) void Init()
{
	if (sigValid)
	{
		// Disable certain outfits removing Sonic's aura
		WRITE_NOP(m_SigSonicAuraVisibility(), 8);
		INSTALL_HOOK(GetCurrentSonicOutfit);

		printf("[Outfit Fixes] m_LoadResModel: 0x%llx\n", m_LoadResModel);
		INSTALL_HOOK(LoadResModel);
	}
	else
	{
		rangersVersionWarning(TEXT("Outfit Fixes"));
	}
}

extern "C" __declspec(dllexport) void PostInit(ModInfo* mods)
{
	if (sigValid)
	{
		// Check for config in loaded mods
		for (Mod* mod : *mods->ModList)
		{
			ModIniData::Data modInfo = ModIniData::Data::ReadConfig(mod->Path);

			// Prevent loading config from this mod
			if (StringHelper::Compare(modInfo.id, "mandm.sonicfrontiers.dlcoutfitfixes"))
				continue;

			std::string configPath = StringHelper::GetSubstringBeforeLastChar(mod->Path, '\\').append("\\OutfitFixes.ini");

			if (Configuration::Load(configPath))
			{
				printf("[Outfit Fixes] Loading configuration from %s\n", mod->Name);

				if (Configuration::incompatible)
					printf("[Outfit Fixes] %s is marked as incompatible, disabling...\n", mod->Name);

				break;
			}
		}
	}
}
