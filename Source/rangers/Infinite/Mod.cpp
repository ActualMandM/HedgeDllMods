#include "CyloopManager.h"
#include "TextManager.h"

extern "C" __declspec(dllexport) void Init()
{
	if (sigValid)
	{
		CyloopManager::init();
		TextManager::init();
	}
	else
	{
		rangersVersionWarning(TEXT("Infinite"));
	}
}
