#include "ArchiveTreePatcher.h"

extern "C" __declspec(dllexport) void Init()
{
	// Swap the image for "You can now switch between the two Sonics."
	WRITE_MEMORY(0x15C5624, uint8_t, 0x70);

	// Swap the mission voice clips
	WRITE_MEMORY(0x01A6D05C, uint8_t, 3313003) /* Amy */
	WRITE_MEMORY(0x01A6D084, uint8_t, 3319003) /* Blaze */
	WRITE_MEMORY(0x01A6D094, uint8_t, 3317003) /* Charmy */
	WRITE_MEMORY(0x01A6D058, uint8_t, 3300003) /* Classic Tails */
	WRITE_MEMORY(0x01A6D074, uint8_t, 3314003) /* Cream */
	WRITE_MEMORY(0x01A6D078, uint8_t, 3316003) /* Espio */
	WRITE_MEMORY(0x01A6D064, uint8_t, 3320003) /* Knuckles */
	WRITE_MEMORY(0x01A6D07C, uint8_t, 3315003) /* Rouge */
	WRITE_MEMORY(0x01A6D080, uint8_t, 3318003) /* Vector */

	ArchiveTreePatcher::applyPatches();
}
