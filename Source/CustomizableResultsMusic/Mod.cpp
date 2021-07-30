#include "Configuration.h"

// Defined here so we don't need to re-write this all the time.
#define ASMHOOK void __declspec(naked)


const char* ResultsChar[] =
{
	"Custom",
	"Sonic1",
	"SCDJP",
	"SCDUS",
	"S3K",
	"SonicR",
	"Adventure",
	"Adventure2",
	"Shadow",
	"Sonic06",
	"Sonic06Town",
	"SecretRings",
	"Unleashed",
	"UnleashedBoss",
	"BlackKnight",
	"S4E1",
	"Colors",
	"ColorsBoss",
	"S4E2",
	"LostWorld",
	"Mania",
	"Forces",
	"SRB2",
	"Persona",
	"Persona2IS",
	"Persona2EP",
	"Persona3",
	"Persona4",
	"Persona5"
};

// This lets Classic have a different song than Modern
// Set these to whatever you want in the config.
char resultStringM[32];
char resultStringC[32];

uint32_t cac_jump = 0x00CFD3CD;

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
		push offset resultStringM
		jmp retrn

		jump :
		pop eax
		push offset resultStringC
		nop

		retrn :
		jmp[cac_jump]
	}
}


void SRankResult(SRank Option)
{
	if (Option == Always)
	{
		WRITE_MEMORY(0xCFD4CA, uint8_t, 0xF8, 0x38);
		WRITE_MEMORY(0xCFD4E8, uint8_t, 0xF8, 0x38);
	}
	else if (Option == Never)
	{
		WRITE_MEMORY(0xCFD4CA, uint8_t, 0x00, 0x39);
		WRITE_MEMORY(0xCFD4E8, uint8_t, 0x00, 0x39);
	}
}

// Swap for Result 1 - No S Rank
char result1StringM[32];
char result1StringC[32];

uint32_t cr1_jump = 0x00CFD4CE;

ASMHOOK CustomResult1()
{
	__asm
	{
		push eax
		mov eax, 0x01E5E304 // Classic sonic singleton, is 0 if modern sonic.
		cmp[eax], 0
		jne jump

		mov eax, 0x01E5E2F8 // Modern sonic singleton, is 0 if classic sonic.
		cmp[eax], 0
		jz jump

		pop eax
		push offset result1StringM
		jmp retrn

		jump :
		pop eax
		push offset result1StringC

		retrn :
		jmp[cr1_jump]
	}
}

// Swap for Result 2 - S Rank
char result2StringM[32];
char result2StringC[32];

uint32_t cr2_jump = 0x00CFD4EC;

ASMHOOK CustomResult2()
{
	__asm
	{
		push eax
		mov eax, 0x01E5E304 // Classic sonic singleton, is 0 if modern sonic.
		cmp[eax], 0
		jne jump

		mov eax, 0x01E5E2F8 // Modern sonic singleton, is 0 if classic sonic.
		cmp[eax], 0
		jz jump

		pop eax
		push offset result2StringM
		jmp retrn

		jump :
		pop eax
		push offset result2StringC

		retrn :
		jmp[cr2_jump]
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
		cmp[eax], 0
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

double SetResultTime(Results result)
{
	double outValue;
	
	switch (result)
	{
	case Sonic06:
	case Sonic06Town:
		outValue = 7.381;
		break;
	case Unleashed:
	case UnleashedBoss:
		outValue = 6.021;
		break;
	case Colors:
	case ColorsBoss:
		outValue = 8.01;
		break;
	case LostWorld:
		outValue = 8.182;
		break;
	case BlackKnight:
		outValue = 10.0;
		break;
	case Custom:
		outValue = (double)Configuration::CustomDuration;
		if (Configuration::OnlyRoundClear) OnlyRoundClear();
		break;
	default:
		if(static_cast<int>(result) >= 0) OnlyRoundClear();
		outValue = *(double*)0x017046C0;
		break;
	}
	
	return outValue;
}

void PrepareStrings(Results resultC, Results resultM)
{
	int resultClassic = (int)resultC;
	int resultModern  = (int)resultM;

	const char* resultsTypeC = resultClassic < 0 ? "" : ResultsChar[resultClassic];
	const char* resultsTypeM = resultModern  < 0 ? "" : ResultsChar[resultModern ];

	strcpy( resultStringC, resultClassic < 0 ? "Result"  : ("RC_" + std::string(resultsTypeC)).c_str());
	strcpy(result1StringC, resultClassic < 0 ? "Result1" : ("R1_" + std::string(resultsTypeC)).c_str());
	strcpy(result2StringC, resultClassic < 0 ? "Result2" : ("R2_" + std::string(resultsTypeC)).c_str());

	strcpy( resultStringM, resultModern  < 0 ? "Result"  : ("RC_" + std::string(resultsTypeM)).c_str());
	strcpy(result1StringM, resultModern  < 0 ? "Result1" : ("R1_" + std::string(resultsTypeM)).c_str());
	strcpy(result2StringM, resultModern  < 0 ? "Result2" : ("R2_" + std::string(resultsTypeM)).c_str());
	
	switch (Configuration::SRankType)
	{
	case Always:
		strcpy(result2StringC, resultClassic < 0 ? "Result1" : ("R1_" + std::string(resultsTypeC)).c_str());
		strcpy(result2StringM, resultModern  < 0 ? "Result1" : ("R1_" + std::string(resultsTypeM)).c_str());
		break;
	case Never:
		strcpy(result1StringC, resultClassic < 0 ? "Result2" : ("R2_" + std::string(resultsTypeC)).c_str());
		strcpy(result1StringM, resultModern  < 0 ? "Result2" : ("R2_" + std::string(resultsTypeM)).c_str());
		break;
	}
}

extern "C" __declspec(dllexport) void Init()
{
	//MessageBoxA(NULL, "ALT + SHIFT + P \n(or CTRL+ALT+P on new launch)", "Attach to VS", MB_ICONWARNING);
	Configuration::load("CRM.ini");

	WRITE_JUMP(0x00CFD3C8, CustomActClear)
	WRITE_JUMP(0x00CFD4C9, CustomResult1)
	WRITE_JUMP(0x00CFD4E7, CustomResult2)
	WRITE_JUMP(0x00CFD55E, CustomResultsTime)
}

extern "C" __declspec(dllexport) void PostInit()
{
	const Results resultC = Configuration::ResultOptionClassic;
	const Results resultM = Configuration::ResultOptionModern;

 	PrepareStrings(resultC, resultM);

	resultTimeC = SetResultTime(resultC);
	resultTimeM = SetResultTime(resultM);
}
