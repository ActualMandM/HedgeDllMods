SIG_SCAN
(
	m_LoadText,
	"\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x20\x48\x8B\x01",
	"xxxx?xxxx?xxxx?xxxx?xxxxxxxxxxxxx"
);

HOOK(int64_t, __fastcall, LoadText, m_LoadText(), int64_t a1, const wchar_t* in_textData, int in_textLength)
{
	in_textData = L"10 years in the joint made you a fucking pussy";
	in_textLength = wcslen(in_textData);

	return originalLoadText(a1, in_textData, in_textLength);
}

extern "C" __declspec(dllexport) void Init()
{
	if (sigValid)
	{
		INSTALL_HOOK(LoadText);
	}
	else
	{
		rangersVersionWarning(TEXT("10Years"));
	}
}
