#include "Configuration.h"

HOOK(void, __fastcall, CPlayerSpeedUpdateParallel, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo)
{
	// Update original function
	originalCPlayerSpeedUpdateParallel(This, _, updateInfo);

	// Contexts and States
	const auto context = This->GetContext();
	const auto stateName = This->m_StateMachine.GetCurrentState()->GetStateName();
	const auto& padState = Sonic::CInputState::GetInstance()->GetPadState();

	// Ints and Floats
	const auto ringCount = context->m_RingCount;
	const auto boostAmount = context->m_ChaosEnergy;
	const auto maxBoostAmount = context->GetMaxChaosEnergy();

	// Booleans
	const auto isSuper = context->StateFlag(eStateFlag_InvokeSuperSonic) == true;
	const auto isGoal = context->StateFlag(eStateFlag_Goal) == true;
	bool isWisp = strstr(stateName.c_str(), "Rocket") || strstr(stateName.c_str(), "Spike");
	bool isTransforming = strstr(stateName.c_str(), "Transform");
	bool isDamaged = strstr(stateName.c_str(), "Damage");
	bool isGrinding = strstr(stateName.c_str(), "Grind");
	bool isTricking = strstr(stateName.c_str(), "Trick");
	bool isDiving = strstr(stateName.c_str(), "Diving");
	bool isModern = *(int*)0x01E5E2F8 > 0 && *(int*)0x01E5E304 == 0;

	// Check if the player can go super based on certain conditions
	bool canSuper = ringCount >= 50;
	bool canTransform = !isGoal && !isWisp && !isTransforming && !isDamaged && !isGrinding && !isTricking && !isDiving && stateName != "HangOn" && stateName != "ExternalControl";

	// TODO: Check story progress and only allow super if the player has either collected all emeralds or beat the final boss
	if (padState.IsTapped(Sonic::eKeyState_Y) && canTransform)
	{
		if (!isSuper && canSuper)
			context->ChangeState("TransformSp");
		else if (isSuper)
			context->ChangeState("TransformStandard");
	}

	// CONFIG: Go back to normal if the stage has been beat
	if (!Configuration::SuperSonicGoal && isGoal && isSuper)
	{
		context->ChangeState("TransformStandard");
		context->ChangeState("Goal"); // Immediately switch back to Goal (prevents softlock from goalring)
	}

	// TODO: Prevent Super Sonic from overfilling boost (this is overkill!)
	if (boostAmount > maxBoostAmount && isSuper)
		DebugDrawText::log("Boost overfilled while super!");
	
	// TODO: Revert some animations being replaced by his floating anim

	// TODO: Force Super Sonic in Perfect Chaos fight (unless Hard Mode)
	//       - Set rings to 50 at start of the stage
	//       - Kill Sonic if he runs out of rings

	// Debug information (through Parameter Editor)
	DebugDrawText::log(format("%s Sonic", isModern ? "Modern" : "Classic"));
	DebugDrawText::log(format("State Name: %s", stateName.c_str()));
	DebugDrawText::log(format("Can Go Super: %s", canSuper && canTransform ? "true" : "false"));
	DebugDrawText::log(format("Super Sonic: %s", isSuper ? "true" : "false"));
	DebugDrawText::log(format("Stage Beaten: %s", isGoal ? "true" : "false"));
}

extern "C" __declspec(dllexport) void Init()
{
	// Check if the configuration file exists
	if (!Configuration::load("BetterSuperSonic.ini"))
	{
		MessageBox(nullptr, TEXT("Failed to load the config file!\nPlease make sure that BetterSuperSonic.ini exists in the mod's folder."),
			TEXT("Better Super Sonic"), MB_ICONERROR);
	}

	// Patch "Decrease Super Sonic Rings Every Second" by brianuuu
	WRITE_MEMORY(0x11D6807, uint8_t, 0xC7, 0x47, 0x64);
	WRITE_MEMORY(0x11D680A, float, 1.0f);
	WRITE_MEMORY(0x11D680E, uint8_t, 0xEB, 0x1D);

	// Patch "Disable Super Sonic Floating Boost" by Skyth
	WRITE_MEMORY(0xDFF34C, uint8_t, 0xEB); // Ground Boost
	WRITE_MEMORY(0xDFE04C, uint8_t, 0xEB); // Air Boost

	INSTALL_HOOK(CPlayerSpeedUpdateParallel);
}
