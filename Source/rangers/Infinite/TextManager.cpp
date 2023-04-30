#include "TextManager.h"

SIG_SCAN
(
	m_LoadText,
	0x140A55A20,
	"\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x20\x48\x8B\x01",
	"xxxx?xxxx?xxxx?xxxx?xxxxxxxxxxxxx"
);

std::string blacklist[] = { "Boom", "Adventure", "Frontiers" };

HOOK(int64_t, __fastcall, LoadText, m_LoadText(), int64_t a1, const wchar_t* in_textData, int in_textLength)
{
	bool textChanged = false;

	std::string textData = StringHelper::WideCharToString(in_textData);

	bool isBlacklisted = StringHelper::ContainsSubstrings(textData, blacklist, sizeof(blacklist) / sizeof(blacklist[0]));

	if (StringHelper::ContainsSubstring(textData, "Sonic") && !isBlacklisted)
	{
		textData = std::regex_replace(textData, std::regex("Super Sonic"), "Phantom Infinite");
		textData = std::regex_replace(textData, std::regex("Sonic"), "Infinite");

		if (StringHelper::ContainsSubstring(textData, "Inugami"))
		{
			textData = "Inugami Korone Palette";
		}

		textChanged = true;
	}

	if (StringHelper::ContainsSubstring(textData, "Rathalos"))
	{
		textData.append(" (unimplemented)");
		textChanged = true;
	}

	if (textChanged)
	{
		in_textData = StringHelper::CharToWideChar(textData.c_str());
		in_textLength = strlen(textData.c_str());
	}

	return originalLoadText(a1, in_textData, in_textLength);
}

void TextManager::init()
{
	INSTALL_HOOK(LoadText);
}
