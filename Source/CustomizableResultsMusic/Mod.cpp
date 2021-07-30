#include "Configuration.h"

// Defined here so we don't need to re-write this all the time.
#define ASMHOOK void __declspec(naked)


// This lets Classic have a different song than Modern
// Set these to whatever you want in the config.
const char* resultStringM = "Result";  // Change this to whatever string you want for Modern's music.
const char* resultStringC = "1up";     // Change this to whatever string you want for Classic's music.

uint32_t mhi_jump = 0x00CFD3CD;

ASMHOOK CustomActClear()
{
	__asm
	{
		push eax
		mov eax, 0x01E5E304 // Classic sonic singleton, is 0 if modern sonic.
		cmp[eax], 0
		jne jump

		mov eax, 0x01E5E2F8 // Modern sonic singleton, is 0 if classic sonic.
		cmp[eax], 0         // Second pass of checking in case something terrible happens (youd be surprised)
		jz jump

		pop eax
		push[resultStringM]
		jmp retrn

		jump :
		pop eax
			push[resultStringC]

			retrn :
			jmp[mhi_jump]
	}
}

// Swaps between Modern and Classic's result times.
// Set these to whatever you want in the config.
double   resultTimeM = 6.099999904632568;  // Modern  music result time, default at 0x017046C0
double   resultTimeC = 7.200000000000000;  // Classic music result time

uint32_t resultTimeJumpOut = 0x00CFD566;

ASMHOOK CustomResultsTime()
{
	__asm
	{
		push eax    // Do this so we can use EAX for whatever

		mov eax, 0x01E5E304 // Classic sonic singleton, is 0 if modern sonic.
		cmp[eax], 0
		jne jump

		mov eax, 0x01E5E2F8 // Modern sonic singleton, is 0 if classic sonic.
		cmp[eax], 0         // Second pass of checking in case something terrible happens (youd be surprised)
		jz jump

		// Modern sonic duration
		lea eax, resultTimeM
		movsd xmm1, [eax]
		jmp retrn

		// Classic sonic duration
		jump :
		lea eax, resultTimeC
			movsd xmm1, [eax]
			jmp retrn


			retrn :
		pop eax
			jmp[resultTimeJumpOut]
	}
}

void OnlyRoundClear()
{
#if _DEBUG
	printf("[Custom Results Music] Round Clear will only play\n");
#endif
	WRITE_MEMORY(0xCFD47F, uint8_t, 0xE9, 0x95, 0x00, 0x00, 0x00, 0xC3, 0x90, 0x90, 0x90);
}

void RoundClearDuration(float duration)
{
#if _DEBUG
	printf("[Custom Results Music] Set duration: %f\n", duration);
#endif
	// Special thanks to brianuuu for finding the address needed for adjusting the duration!
	WRITE_MEMORY(0x17046C0, double, duration);
}

extern "C" __declspec(dllexport) void Init()
{
	Configuration::load("mod.ini");

	switch (Configuration::ResultOption)
	{
		case S06:
		case S06Town:
			RoundClearDuration(7.381f);
			break;
		case Unleashed:
		case UnleashedBoss:
			RoundClearDuration(6.021f);
			break;
		case Colors:
		case ColorsBoss:
			RoundClearDuration(8.01f);
			break;
		case SLW:
			RoundClearDuration(8.182f);
			break;
		case BlackKnight:
			RoundClearDuration(10.0f);
			break;
		case Custom:
			RoundClearDuration(Configuration::CustomDuration);
			break;
	}

	WRITE_JUMP(0x00CFD3C8, CustomActClear);
	WRITE_JUMP(0x00CFD55E, CustomResultsTime);
}
