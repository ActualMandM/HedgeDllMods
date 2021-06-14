#include "ArchiveTreePatcher.h"

extern "C" __declspec(dllexport) void Init()
{
	// Swap the image for "You can now switch between the two Sonics."
	WRITE_MEMORY(0x15C5624, uint8_t, 0x70);

	ArchiveTreePatcher::applyPatches();
}
