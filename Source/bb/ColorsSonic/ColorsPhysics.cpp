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
		WRITE_MEMORY(0x1232056, char*, "AirBoost");

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
	if (isAirBoostToFall)
	{
		// Don't transition animation
		WRITE_MEMORY(0x111910F, uint8_t, 0xEB);
		WRITE_MEMORY(0x1118E0C, uint8_t, 0xEB);
		WRITE_MEMORY(0x1118EA7, uint8_t, 0xEB);
	}
	else
	{
		// Revert
		WRITE_MEMORY(0x111910F, uint8_t, 0x75);
		WRITE_MEMORY(0x1118E0C, uint8_t, 0x75);
		WRITE_MEMORY(0x1118EA7, uint8_t, 0x76);
	}
	return originalColorsPhysics_CSonicStateFallBegin(This);
}

HOOK(int, __fastcall, ColorsPhysics_CSonicStateFallEnd, 0x1118F20, hh::fnd::CStateMachineBase::CStateBase* This)
{
	isAirBoostToFall = false;
	return originalColorsPhysics_CSonicStateFallEnd(This);
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
	INSTALL_HOOK(ColorsPhysics_CSonicStateFallEnd);

	// Always kill boost effect immediately
	WRITE_NOP(0xDFB32D, 2);
	WRITE_NOP(0xDFB39A, 2);
}
