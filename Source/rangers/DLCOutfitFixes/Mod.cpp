#include "Mod.h"
#include "Configuration.h"

/* v1.40: 0x140132A20 */
SIG_SCAN
(
	m_SigGetCurrentSonicOutfit,
	"\x0F\xBE\xC1\xFF\xC8\x83\xF8\x05\x77\x24\x48\x8D\x15\x00\x00\x00\x00\x48\x98\x8B\x8C\x82\x00\x00\x00\x00\x48\x03\xCA\xFF\xE1\xB0\x01\xC3\xB0\x02\xC3\xB0\x04\xC3\xB0\x07\xC3\xB0\x08\xC3",
	"xxxxxxxxxxxxx????xxxxx????xxxxxxxxxxxxxxxxxxxx"
);

/* v1.40: 0x14A69E1F5 */
SIG_SCAN
(
	m_SigGetCurrentFriendOutfit,
	"\xE8\x00\x00\x00\x00\x84\xC0\x48\x8D\x0D\x00\x00\x00\x00\x48\x8D\x3D\x00\x00\x00\x00\xB2\x01\x48\x0F\x44\xCF\x0F\xB6\xF0\xE8\x00\x00\x00\x00\xB2\x01\x48\x89\x43\x38\x48\x89\xF9",
	"x????xxxxx????xxx????xxxxxxxxxx????xxxxxxxxx"
);

uint8_t sonicOutfit = 0;
bool friendOutfit = false;

HOOK(uint8_t, __fastcall, GetCurrentSonicOutfit, m_SigGetCurrentSonicOutfit(), uint8_t in_saveOutfitIdx)
{
	sonicOutfit = in_saveOutfitIdx;
	return originalGetCurrentSonicOutfit(in_saveOutfitIdx);
}

/* v1.40: 0x1401341B0 */
auto m_GetCurrentFriendsOutfit = READ_CALL(m_SigGetCurrentFriendOutfit());

HOOK(bool, __fastcall, GetCurrentFriendsOutfit, m_GetCurrentFriendsOutfit, int64_t a1)
{
	bool result = originalGetCurrentFriendsOutfit(a1);
	friendOutfit = result;
	return result;
}

/* v1.40: 0x140912890 */
SIG_SCAN
(
	m_SigSonicAuraVisibility,
	"\x3C\x02\x74\x16\x3C\x08\x74\x12\xB2\x01\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x89\x00\x40",
	"xxxxxxxxxxxxx????x????xx?x"
);

/* v1.40: 0x1401584A0 */
auto m_LoadResModel = READ_CALL(m_SigSonicAuraVisibility() - 0x1A);

HOOK(int64_t, __fastcall, LoadResModel, m_LoadResModel, const char* in_modelName, int64_t a2)
{
	char modelName[256];
	strcpy(modelName, in_modelName);

	if (!Configuration::incompatible && StringHelper::ContainsSubstring(modelName, "chr_"))
	{
		// Sonic
		if (!Configuration::sonicIncompatible && (sonicOutfit > 0 && sonicOutfit <= MAX_OUTFIT) &&
			StringHelper::ContainsSubstring(modelName, "sonic"))
		{
			uint8_t modelIdx = sonicOutfit - 1;

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
		if (!Configuration::friendsIncompatible && friendOutfit &&
			(StringHelper::ContainsSubstring(modelName, "amy") || StringHelper::ContainsSubstring(modelName, "knuckles") || StringHelper::ContainsSubstring(modelName, "tails")))
		{
			if (StringHelper::Compare(in_modelName, "chr_amyP"))
				GetFriendName(modelName, "amy", "P");

			else if (StringHelper::Compare(in_modelName, "chr_knucklesP"))
				GetFriendName(modelName, "knuckles", "P");

			else if (StringHelper::Compare(in_modelName, "chr_tailsP"))
				GetFriendName(modelName, "tails", "P");
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

		// Install hooks for getting outfits
		INSTALL_HOOK(GetCurrentSonicOutfit);
		printf("[Outfit Fixes] m_GetCurrentFriendsOutfit: 0x%llx\n", m_GetCurrentFriendsOutfit);
		INSTALL_HOOK(GetCurrentFriendsOutfit);

		// Install hook for loading ResModel
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
				{
					printf("[Outfit Fixes] %s is marked as incompatible, disabling.\n", mod->Name);
					break;
				}
			}
		}
	}
}
