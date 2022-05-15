#include "Configuration.h"

bool bpcTransformed = false;

HOOK(void, __fastcall, CPlayerSpeedUpdateParallel, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo)
{
	// TODO: Check save file and only run this code if the player has beaten the final boss (Configuration::saveCheck)
	//       - Hidden configuration option, set to false for the code to always execute

	// TODO: Play invincibility/custom music when super if config is set to do so (Configuration::superMusic)
	//       - This will more than likely require another hook to prevent spamming
	//       - (multiple hooks is probably better for long term anyways lol)

	// Contexts and States
	const auto context = This->GetContext();
	const auto stateName = This->m_StateMachine.GetCurrentState()->GetStateName();
	const auto& padState = Sonic::CInputState::GetInstance()->GetPadState();

	// Ints and Floats
	uint32_t& ringCount = context->m_RingCount;
	float& boostAmount = context->m_ChaosEnergy;
	const float maxBoostAmount = context->GetMaxChaosEnergy();

	// Booleans
	const bool isBPC = Configuration::bpcSuper && Helpers::CheckCurrentStage("bpc");
	const bool isSuper = context->StateFlag(eStateFlag_InvokeSuperSonic);
	const bool isOutOfControl = context->StateFlag(eStateFlag_OutOfControl);
	const bool isGoal = context->StateFlag(eStateFlag_Goal);
	const bool isWisp = strstr(stateName.c_str(), "Rocket") || strstr(stateName.c_str(), "Spike");
	const bool isTransforming = strstr(stateName.c_str(), "Transform");
	const bool isGrinding = strstr(stateName.c_str(), "Grind");
	const bool isDiving = strstr(stateName.c_str(), "Diving");
	const bool isDead = strstr(stateName.c_str(), "Dead");
	const bool isModern = *(uint32_t*)This == 0x16D4B2C;

	// Check if the player can go super based on certain conditions
	// TODO: Use the same check that the skill uses for whether or not the player can transform into super
	const bool canSuper = ringCount >= 50;
	const bool canTransform = !isOutOfControl && !isGoal && !isWisp && !isTransforming && !isGrinding && !isDiving && stateName != "HangOn" && stateName != "ExternalControl";

#if _DEBUG
	// Debug options and information (through Parameter Editor)
	if (isModern)
	{
		if (padState.IsTapped(Sonic::eKeyState_DpadUp))
			ringCount += 50;
		else if (padState.IsTapped(Sonic::eKeyState_DpadDown) && ringCount >= 50)
			ringCount -= 50;

		if (padState.IsTapped(Sonic::eKeyState_DpadRight))
			boostAmount += 25.0f;
		else if (padState.IsTapped(Sonic::eKeyState_DpadLeft))
			boostAmount -= 25.0f;
	}
	else
	{
		if (padState.IsTapped(Sonic::eKeyState_RightBumper))
			ringCount += 50;
		else if (padState.IsTapped(Sonic::eKeyState_LeftBumper) && ringCount >= 50)
			ringCount -= 50;
	}

	DebugDrawText::log(format("%s Sonic", isModern ? "Modern" : "Classic"));
	DebugDrawText::log(format("State Name: %s", stateName.c_str()));
	DebugDrawText::log(format("Can Go Super: %s", canSuper && canTransform ? "true" : "false"));
	DebugDrawText::log(format("Super Sonic: %s", isSuper ? "true" : "false"));
	DebugDrawText::log(format("Stage Beaten: %s", isGoal ? "true" : "false"));
	DebugDrawText::log(format("Boost Amount: %.0f", boostAmount));
#endif

	// CONFIG: Super Sonic in Perfect Chaos boss
	if (isBPC && !isOutOfControl)
	{
		if (!isSuper && !isTransforming && !bpcTransformed && !isDead)
		{
			ringCount = 50;
			context->ChangeState("TransformSp");
			bpcTransformed = true;
		}
		else if (ringCount == 0)
			This->SendMessage(This->m_ActorID, boost::make_shared<Sonic::Message::MsgDead>(false));

		if (isDead)
			bpcTransformed = false;

		// Update original function
		originalCPlayerSpeedUpdateParallel(This, _, updateInfo);

		return;
	}
	else if (bpcTransformed)
		bpcTransformed = false;

	if (padState.IsTapped(Sonic::eKeyState_Y) && canTransform)
	{
		// TODO: Check whether or not the Super Sonic skill is equipped
		if (!isSuper && canSuper && !Configuration::skillOnly)
			context->ChangeState("TransformSp");
		else if (isSuper && Configuration::superSonicToggle)
			context->ChangeState("TransformStandard");
	}

	// CONFIG: Go back to normal if the stage has been beat
	if (isGoal && isSuper && !isBPC)
	{
		if ((Configuration::goalType == Classic && !isModern) || (Configuration::goalType == Modern && isModern) || Configuration::goalType == Both)
		{
			context->ChangeState("TransformStandard");

			if (!Helpers::IsCurrentStageMission())
				context->ChangeState("Goal"); // Immediately switch back to Goal (prevents softlock from goalring)
			else
				context->StateFlag(eStateFlag_MoveToPositionAndWait) = true; // Re-enable flag to remove control from the player
		}
	}

	// TODO: Revert some animations being replaced by his floating anim
	//       - Probably will require ArchiveTreePatcher shenagigans

	// Update original function
	originalCPlayerSpeedUpdateParallel(This, _, updateInfo);

	// Prevent Super Sonic from overfilling boost (this is overkill!)
	if (isSuper)
		boostAmount = std::clamp(boostAmount, 0.0f, maxBoostAmount);
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
	// TODO: Make this work for Classic as well
	WRITE_MEMORY(0x11D6807, uint8_t, 0xC7, 0x47, 0x64);
	WRITE_MEMORY(0x11D680A, float, 1.0f);
	WRITE_MEMORY(0x11D680E, uint8_t, 0xEB, 0x1D);

	// Patch "Disable Super Sonic Floating Boost" by Skyth
	WRITE_MEMORY(0xDFF34C, uint8_t, 0xEB); // Ground Boost
	WRITE_MEMORY(0xDFE04C, uint8_t, 0xEB); // Air Boost

	INSTALL_HOOK(CPlayerSpeedUpdateParallel);
}
