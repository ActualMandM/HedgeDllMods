#include "Mod.h"

SIG_SCAN
(
	m_SigGetCurrentOutfit,
	0x140117E40,
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
	0x14080F16B,
	"\x3C\x02\x74\x16\x3C\x08\x74\x12\xB2\x01\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x89\x43\x40",
	"xxxxxxxxxxxxx????x????xxxx"
);

auto m_LoadResModel = READ_CALL(m_SigSonicAuraVisibility() + 0x11);

HOOK(__int64, __fastcall, LoadResModel, m_LoadResModel, const char* in_modelName, __int64 a2)
{
	if (outfit != 0 && StringHelper::ContainsSubstring(in_modelName, "chr_"))
	{
		uint8_t modelIdx = outfit - 1;

		if (!strcmp(in_modelName, "chr_sonic_aura"))
			in_modelName = auraNames[modelIdx];

		if (!strcmp(in_modelName, "chr_sonic_shape"))
			in_modelName = shapeNames[modelIdx];

		if (!strcmp(in_modelName, "chr_supersonic"))
			in_modelName = superNames[modelIdx];

		if (!strcmp(in_modelName, "chr_supersonic_aura"))
			in_modelName = superAuraNames[modelIdx];

		if (!strcmp(in_modelName, "chr_supersonic_shape"))
			in_modelName = superShapeNames[modelIdx];
	}

	return originalLoadResModel(in_modelName, a2);
}

extern "C" __declspec(dllexport) void Init()
{
	if (sigValid)
	{
		WRITE_NOP(m_SigSonicAuraVisibility(), 8);
		INSTALL_HOOK(GetCurrentOutfit);

		printf("[DLC Outfit Fixes] m_LoadResModel: 0x%llx\n", m_LoadResModel);
		INSTALL_HOOK(LoadResModel);
	}
	else
	{
		rangersVersionWarning(TEXT("DLC Outfit Fixes"));
	}
}
