#include "TextManager.h"

SIG_SCAN
(
	m_LoadText,
	"\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x20\x48\x8B\x01",
	"xxxx?xxxx?xxxx?xxxx?xxxxxxxxxxxxx"
);

std::wstring blacklist[] = { L"Boom", L"Adventure", L"Frontiers" };

HOOK(int64_t, __fastcall, LoadText, m_LoadText(), int64_t a1, const wchar_t* in_textData, int in_textLength)
{
	bool textChanged = false;

	std::wstring textData = in_textData;

	bool isBlacklisted = StringHelper::ContainsSubstrings(textData, blacklist, sizeof(blacklist) / sizeof(blacklist[0]));

	if (StringHelper::ContainsSubstring(textData, L"Sonic") && !isBlacklisted)
	{
		textData = std::regex_replace(textData, std::wregex(L"Super Sonic"), L"Phantom Infinite");
		textData = std::regex_replace(textData, std::wregex(L"Sonic"), L"Infinite");

		if (StringHelper::ContainsSubstring(textData, L"Inugami"))
		{
			textData = L"Inugami Korone Palette";
		}

		textChanged = true;
	}

	if (StringHelper::ContainsSubstring(textData, L"Rathalos"))
	{
		textData.append(L" (unimplemented)");
		textChanged = true;
	}

	if (textChanged)
	{
		in_textData = textData.c_str();
		in_textLength = textData.size();
	}

	return originalLoadText(a1, in_textData, in_textLength);
}

void TextManager::init()
{
	INSTALL_HOOK(LoadText);
}
