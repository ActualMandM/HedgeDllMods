#include "CyloopManager.h"

inline float luminance = 0.8;
inline float luminanceFlash = 0.5;

void CyloopManager::changeColorF(colorF& color, float red, float green, float blue)
{
	color.red = red;
	color.green = green;
	color.blue = blue;
}

void CyloopManager::changeCyloopColor(CyloopLocusParameter& locus)
{
	for (int i = 0; i < locus.m_numTransparentLines; i++)
	{
		auto& transparentLine = locus.m_transparentLines[i];

		changeColorF(transparentLine.m_color0, 1, 0.05, 0.05);
		changeColorF(transparentLine.m_color1, 1, 0.05, 0.05);

		changeColorF(transparentLine.m_flashColor0, 0.7, 0.05, 0.2);
		changeColorF(transparentLine.m_flashColor1, 0.7, 0.05, 0.2);

		transparentLine.m_luminance = luminance;
		transparentLine.m_flashLuminance = luminanceFlash;
	}

	for (int i = 0; i < locus.m_numOpaqueLines; i++)
	{
		auto& opaqueLine = locus.m_opaqueLines[i];

		changeColorF(opaqueLine.m_color, 1, 0.05, 0.05);
		changeColorF(opaqueLine.m_flashColor, 0.7, 0.05, 0.2);
	}

	auto& crossLine = locus.m_crossline;
	{
		changeColorF(crossLine.startColor, 1, 0, 0);
		changeColorF(crossLine.endColor, 0.9, 0.1, 0.1);

		changeColorF(crossLine.startColorFlash, 0.7, 0, 0.2);
		changeColorF(crossLine.endColorFlash, 0.6, 0.1, 0.1);

		crossLine.startColorLuminance = luminance;
		crossLine.endColorLuminance = luminance;

		crossLine.startColorLuminanceFlash = luminanceFlash;
		crossLine.endColorLuminanceFlash = luminanceFlash;
	}
}

SIG_SCAN
(
	m_SigLoadAsset,
	"\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x81\xEC\xD0\x00\x00\x00\x49\x8B\xF8",
	"xxxxxxxxxxxxxxxxxxxxx"
);

HOOK(int64_t, __fastcall, LoadAsset, m_SigLoadAsset(), int64_t a1, const char* in_assetName, const char** in_resourceType)
{
	auto result = originalLoadAsset(a1, in_assetName, in_resourceType);

	if (StringHelper::Compare(*in_resourceType, "ResReflection") && StringHelper::Compare(in_assetName, "player_common"))
	{
		auto sonicParameter = reinterpret_cast<SonicParameters*>(*(int64_t*)(result + 0x60));

		if (sonicParameter)
		{
			auto& cyloop = sonicParameter->common.cyloop;

			CyloopManager::changeCyloopColor(cyloop.locus);
			CyloopManager::changeCyloopColor(cyloop.locusQuick);
			CyloopManager::changeCyloopColor(cyloop.locusSuperSonic);
			CyloopManager::changeCyloopColor(cyloop.locusSuperSonicQuick);
		}
	}

	return result;
}

void CyloopManager::init()
{
	INSTALL_HOOK(LoadAsset);
}
