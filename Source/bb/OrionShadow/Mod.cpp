#include "Glitter.h"
#include "MathUtil.h"
#include "SonicParticle.h"

boost::shared_ptr<Hedgehog::Glitter::CHandle> JetHandleL;
boost::shared_ptr<Sonic::CMatrixNodeTransform> JetNodeL;
boost::shared_ptr<Hedgehog::Glitter::CHandle> JetHandleR;
boost::shared_ptr<Sonic::CMatrixNodeTransform> JetNodeR;

HOOK(void, __fastcall, _ClassicUpdate, 0xDDABA0, Sonic::Player::CSonicClassic* This, void*, const hh::fnd::SUpdateInfo& in_rUpdateInfo)
{
	original_ClassicUpdate(This, 0, in_rUpdateInfo);

	using namespace hh::math;

	Sonic::CGameDocument* pGameDocument = **Sonic::CGameDocument::GetInstance();
	if (!JetHandleL && !JetHandleR)
	{
		JetNodeL = boost::make_shared<Sonic::CMatrixNodeTransform>();
		JetNodeR = boost::make_shared<Sonic::CMatrixNodeTransform>();
		JetNodeL->m_Transform.SetRotationAndPosition(CQuaternion::FromAngleAxis(90.0f * MathUtil::DegreesToRadians, CVector::Forward()), CVector::Zero());
		JetNodeR->m_Transform.SetRotationAndPosition(CQuaternion::FromAngleAxis(90.0f * MathUtil::DegreesToRadians, CVector::Forward()), CVector::Zero());
		JetNodeL->SetParent(This->m_spCharacterModel->GetNode("Foot_L").get());
		JetNodeR->SetParent(This->m_spCharacterModel->GetNode("Foot_R").get());
		JetHandleL = pGameDocument->m_pMember->m_pParticleManager->AddParticle(JetNodeL, "ef_classic_shadow_jet_shoe");
		JetHandleR = pGameDocument->m_pMember->m_pParticleManager->AddParticle(JetNodeR, "ef_classic_shadow_jet_shoe");
	}

	float velocityThreshold = This->GetContext()->m_spParameter->Get<float>(Sonic::Player::eSonicClassicParameter_MotionRunToDash);

	if (This->GetContext()->GetCurrentAnimationName() == "Walk" && This->GetVelocityForAnimationChange().norm() >= velocityThreshold)
	{
		JetHandleL->m_ParticleScale = CVector::One();
		JetHandleR->m_ParticleScale = CVector::One();
	}
	else
	{
		JetHandleL->m_ParticleScale = CVector::Zero();
		JetHandleR->m_ParticleScale = CVector::Zero();
	}
}

HOOK(void, __fastcall, MsgRestartStage, 0xE76810, Sonic::Player::CPlayer* This, void* Edx, hh::fnd::Message& message)
{
	originalMsgRestartStage(This, 0, message);
	JetNodeL = nullptr;
	JetNodeR = nullptr;
	JetHandleL = nullptr;
	JetHandleR = nullptr;
}

HOOK(void, __fastcall, SonicRemoveCallback, 0xDD7900, Sonic::Player::CPlayer* This, void* Edx, void* A1)
{
	originalSonicRemoveCallback(This, 0, A1);
	JetNodeL = nullptr;
	JetNodeR = nullptr;
	JetHandleL = nullptr;
	JetHandleR = nullptr;
}

extern "C" __declspec(dllexport) void Init(ModInfo* mod)
{
	INIReader reader(mod->CurrentMod->Path);

	if (!_stricmp(reader.Get("Main", "IncludeDir1", "dummy").c_str(), "disk_skate"))
	{
		INSTALL_HOOK(_ClassicUpdate);
		INSTALL_HOOK(MsgRestartStage);
		INSTALL_HOOK(SonicRemoveCallback);
	}
}
