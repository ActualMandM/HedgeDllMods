#include "ColorsPhysics.h"
#include "Configuration.h"
#include "AnimationSetPatcher.h"

using SharedPtrTypeless = boost::shared_ptr<void>;
static void* fCGlitterCreate
(
	void* pContext,
	SharedPtrTypeless& handle,
	void* pMatrixTransformNode,
	Hedgehog::Base::CSharedString const& name,
	uint32_t flag
)
{
	static void* const pCGlitterCreate = (void*)0xE73890;
	__asm
	{
		push    flag
		push    name
		push    pMatrixTransformNode
		mov     eax, pContext
		mov     esi, handle
		call	[pCGlitterCreate]
	}
}

static void fCGlitterEnd
(
	void* pContext,
	SharedPtrTypeless& handle,
	bool instantStop
)
{
	static void* const pCGlitterEnd = (void*)0xE72650;
	static void* const pCGlitterKill = (void*)0xE72570;
	__asm
	{
		mov     eax, [handle]
		mov     ebx, [eax + 4]
		push    ebx
		test	ebx, ebx
		jz		noIncrement
		mov		edx, 1
		add		ebx, 4
		lock xadd [ebx], edx

		noIncrement:
		mov     ebx, [eax]
		push    ebx
		mov     eax, pContext
		cmp     instantStop, 0
		jnz     jump
		call	[pCGlitterEnd]
		jmp     end

		jump:
		call	[pCGlitterKill]

		end:
	}
}

bool homingDummy = false;
SharedPtrTypeless jumpBallPfxHandle;
const char* volatile const AirBoost = "AirBoost";
HOOK(int, __fastcall, ColorsPhysics_CSonicStateHomingAttackBegin, 0x1232040, hh::fnd::CStateMachineBase::CStateBase* This)
{
	auto* context = (Sonic::Player::CPlayerSpeedContext*)This->GetContextBase();
	if (context->m_HomingAttackTargetActorID)
	{
		homingDummy = false;
		WRITE_MEMORY(0x1232056, char*, AnimationSetPatcher::JumpBallNoBlend);

		void* matrixNode = (void*)((uint32_t)context + 0x30);
		fCGlitterCreate(context, jumpBallPfxHandle, matrixNode, context->StateFlag(eStateFlag_InvokeSuperSonic) ? "ef_ch_sps_yh1_spinattack" : "ef_ch_sng_yh1_spinattack", 4);

		// Revert locus effect
		WRITE_MEMORY(0x1232508, uint8_t, 0x6A, 0x00, 0x8B, 0xC3, 0xE8);
	}
	else
	{
		homingDummy = true;
		WRITE_MEMORY(0x1232056, char*, AirBoost);

		// Don't use locus effect
		WRITE_JUMP(0x1232508, (void*)0x1232511);
	}
	return originalColorsPhysics_CSonicStateHomingAttackBegin(This);
}

HOOK(void, __fastcall, ColorsPhysics_CSonicStateHomingAttackEnd, 0x1231F80, hh::fnd::CStateMachineBase::CStateBase* This)
{
	auto* context = (Sonic::Player::CPlayerSpeedContext*)This->GetContextBase();
	if (!homingDummy)
	{
		fCGlitterEnd(context, jumpBallPfxHandle, true);
	}
	originalColorsPhysics_CSonicStateHomingAttackEnd(This);
}

bool isAirBoostToFall = false;
bool isBoostAtFallBegin = false;
void __declspec(naked) ColorsPhysics_AirBoostToFall()
{
    static uint32_t returnAddress = 0x1233330;
    __asm
    {
		mov		isAirBoostToFall, 1
		push	[0x15F5030] // Fall
        jmp     [returnAddress]
    }
}

HOOK(int, __fastcall, ColorsPhysics_CSonicStateFallBegin, 0x1118FB0, hh::fnd::CStateMachineBase::CStateBase* This)
{
	auto* context = (Sonic::Player::CPlayerSpeedContext*)This->GetContextBase();
	isBoostAtFallBegin = context->StateFlag(eStateFlag_Boost) && !isAirBoostToFall;

	if (isAirBoostToFall || isBoostAtFallBegin)
	{
		// Don't transition animation
		WRITE_MEMORY(0x111910F, uint8_t, 0xEB);
		WRITE_MEMORY(0x1118DE5, uint8_t, 0xEB);
		WRITE_MEMORY(0x1118E94, uint8_t, 0xEB);

		if (isBoostAtFallBegin)
		{
			context->ChangeAnimation(AirBoost);
		}
	}
	else
	{
		// Revert
		WRITE_MEMORY(0x111910F, uint8_t, 0x75);
		WRITE_MEMORY(0x1118DE5, uint8_t, 0x75);
		WRITE_MEMORY(0x1118E94, uint8_t, 0x76);
	}
	return originalColorsPhysics_CSonicStateFallBegin(This);
}

HOOK(void, __fastcall, ColorsPhysics_CSonicStateFallAdvance, 0x1118C50, hh::fnd::CStateMachineBase::CStateBase* This)
{
	auto* context = (Sonic::Player::CPlayerSpeedContext*)This->GetContextBase();
	if (isBoostAtFallBegin && !context->StateFlag(eStateFlag_Boost))
	{
		isBoostAtFallBegin = false;
		context->ChangeAnimation("Fall");

		// Allow change animation to Fall/FallLarge
		WRITE_MEMORY(0x1118DE5, uint8_t, 0x75);
		WRITE_MEMORY(0x1118E94, uint8_t, 0x76);
	}

	return originalColorsPhysics_CSonicStateFallAdvance(This);
}

HOOK(int, __fastcall, ColorsPhysics_CSonicStateFallEnd, 0x1118F20, hh::fnd::CStateMachineBase::CStateBase* This)
{
	isAirBoostToFall = false;
	isBoostAtFallBegin = false;
	return originalColorsPhysics_CSonicStateFallEnd(This);
}

std::vector<StartData> ColorsPhysics::m_startCrouchingData;
size_t startCrouchingIndex = 0;
std::vector<StartData> ColorsPhysics::m_startStandingData;
size_t startStandingIndex = 0;
HOOK(bool, __stdcall, ColorsPhysics_StartSequence, 0xDFCE30, uint32_t* This)
{
	switch (This[1329])
	{
	case 2: // StartEventDash
	case 3:
	{
		for (StartData& data : ColorsPhysics::m_startCrouchingData)
		{
			data.reset();
		}

		startCrouchingIndex = (startCrouchingIndex + 1) % ColorsPhysics::m_startCrouchingData.size();
		char const* animationName = ColorsPhysics::m_startCrouchingData[startCrouchingIndex].m_animationName.c_str();
		WRITE_MEMORY(0xDFD0AB, char*, animationName);
		WRITE_MEMORY(0xDFD1CA, char*, animationName);
		break;
	}
	case 4: // StartEventStand
	{
		for (StartData& data : ColorsPhysics::m_startStandingData)
		{
			data.reset();
		}

		startStandingIndex = (startStandingIndex + 1) % ColorsPhysics::m_startStandingData.size();
		char const* animationName = ColorsPhysics::m_startStandingData[startStandingIndex].m_animationName.c_str();
		WRITE_MEMORY(0xDFD2C2, char*, animationName);
		break;
	}
	}

	return originalColorsPhysics_StartSequence(This);
}

HOOK(void, __fastcall, ColorsPhysics_CSonicStateStartCrouchingAdvance, 0xDEF180, hh::fnd::CStateMachineBase::CStateBase* This)
{
	auto* context = (Sonic::Player::CPlayerSpeedContext*)This->GetContextBase();

	StartData& data = ColorsPhysics::m_startCrouchingData[startCrouchingIndex];
	for (StartSoundData& soundData : data.m_soundData)
	{
		if (!soundData.m_played && This->m_Time >= soundData.m_soundTiming)
		{
			soundData.m_played = true;
			context->PlaySound(soundData.m_cueID, 1);
		}
	}

	originalColorsPhysics_CSonicStateStartCrouchingAdvance(This);
}

HOOK(void, __fastcall, ColorsPhysics_CSonicStateStartStandingAdvance, 0xDEE070, hh::fnd::CStateMachineBase::CStateBase* This)
{
	auto* context = (Sonic::Player::CPlayerSpeedContext*)This->GetContextBase();

	StartData& data = ColorsPhysics::m_startStandingData[startStandingIndex];
	for (StartSoundData& soundData : data.m_soundData)
	{
		if (!soundData.m_played && This->m_Time >= soundData.m_soundTiming)
		{
			soundData.m_played = true;
			context->PlaySound(soundData.m_cueID, 1);
		}
	}

	originalColorsPhysics_CSonicStateStartStandingAdvance(This);
}

void ColorsPhysics::applyPatches()
{
	static float homingDummyAfterSpeedMultiplier = 0.8f;
	WRITE_MEMORY(0x1231B0D, float*, &homingDummyAfterSpeedMultiplier);

	INSTALL_HOOK(ColorsPhysics_CSonicStateHomingAttackBegin);
	INSTALL_HOOK(ColorsPhysics_CSonicStateHomingAttackEnd);

	// After AirBoost it won't change to fall animation
	WRITE_JUMP(0x123332B, ColorsPhysics_AirBoostToFall);
	INSTALL_HOOK(ColorsPhysics_CSonicStateFallBegin);
	INSTALL_HOOK(ColorsPhysics_CSonicStateFallAdvance);
	INSTALL_HOOK(ColorsPhysics_CSonicStateFallEnd);

	// Always kill boost effect immediately
	WRITE_NOP(0xDFB32D, 2);
	WRITE_NOP(0xDFB39A, 2);

	m_startCrouchingData =
	{
		{
			// sn_start_normal07
			"StartEventDash",
			{
				{ 1.839f, 5002098, false },
				{ 2.335f, 5002099, false },
				{ 2.914f, 5002100, false },
			}
		},
		{
			// sn_start_normal00
			"StartEventDash2",
			{
				{ 0.597f, 5002089, false },
				{ 0.877f, 5002090, false },
			}
		},
	};

	m_startStandingData =
	{
		{
			"StartEventStand", {}
		},
		{
			// sn_start_wait00_sv
			"StartEventStand2",
			{
				{ 1.280f, 5002105, false },
				{ 1.877f, 5002106, false },
				{ 2.378f, 5002107, false },
				{ 0.740f, 5002108, false },
				{ 0.731f, 5002109, false },
			}
		},
	};

	// Start animation sfx
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	startCrouchingIndex = std::rand() % ColorsPhysics::m_startCrouchingData.size();
	startStandingIndex = std::rand() % ColorsPhysics::m_startStandingData.size();
	INSTALL_HOOK(ColorsPhysics_StartSequence);
	INSTALL_HOOK(ColorsPhysics_CSonicStateStartCrouchingAdvance);
	INSTALL_HOOK(ColorsPhysics_CSonicStateStartStandingAdvance);
}
