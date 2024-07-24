#pragma once

namespace Sonic
{
	static void __declspec(naked) __declspec(noinline) fAddParticle()
	{
		static constexpr int fn = 0xE8F8A0;
		__asm
		{
			mov eax, ecx

			jmp[fn]
		}
	}

	static BB_FUNCTION_PTR(void*, __thiscall, fpAddParticle, fAddParticle, Sonic::CParticleManager* particleManager,
		boost::shared_ptr<Hedgehog::Glitter::CHandle>* out_pHandle,
		const boost::shared_ptr<Hedgehog::Mirage::CMatrixNode>& in_rNode, const Hedgehog::Base::CSharedString& in_rName, int in_Flag);

	static void __declspec(naked) __declspec(noinline) fStopParticle()
	{
		static constexpr int fn = 0xE72650;
		__asm
		{
			mov eax, ecx
			jmp[fn]
		}
	}
	static BB_FUNCTION_PTR(void*, __thiscall, fpStopParticle, fStopParticle,
		Sonic::CParticleManager* particleManager, boost::shared_ptr<Hedgehog::Glitter::CHandle> in_Handle);

	static void __declspec(naked) __declspec(noinline) fKillParticle()
	{
		static constexpr int fn = 0xE72570;
		__asm
		{
			mov eax, ecx
			jmp[fn]
		}
	}
	static BB_FUNCTION_PTR(void*, __thiscall, fpKillParticle, fKillParticle,
		Sonic::CParticleManager* particleManager, boost::shared_ptr<Hedgehog::Glitter::CHandle> in_Handle);


	class CParticleController : public Hedgehog::Universe::CMessageActor
	{
	public:
		int m_Field07C;
		hh::map<void, void>* m_ParticleMap;
		int m_Field084;
		int m_pParticleManagerID;
		int m_Field08C;
	};

	class CParticleManager : public CGameObject
	{
	public:

		boost::shared_ptr<Hedgehog::Glitter::CHandle>
			AddParticle(const boost::shared_ptr<Hedgehog::Mirage::CMatrixNode>& in_rNode, const Hedgehog::Base::CSharedString& in_rName, int in_Flag = 1)
		{
			boost::shared_ptr<Hedgehog::Glitter::CHandle> spHandle;
			fpAddParticle(this, &spHandle, in_rNode, in_rName, in_Flag);
			return spHandle;
		}

		void StopParticle(boost::shared_ptr<Hedgehog::Glitter::CHandle>& in_rHandle)
		{
			fpStopParticle(this, in_rHandle);
		}

		void KillParticle(boost::shared_ptr<Hedgehog::Glitter::CHandle>& in_rHandle)
		{
			fpKillParticle(this, in_rHandle);
		}

		int m_Field0A8;
		Hedgehog::Glitter::CManager* m_pGlitterManager;
		int m_Field0B0;
		int m_Field0B4;
		int m_Field0B8;
		int m_Field0BC;
		int m_Field0C0;
		int m_Field0C4;
		int m_Field0C8;
		int m_Field0CC;
		int m_Field0D0;
		int m_Field0D4;
		int m_Field0D8;
		int m_Field0DC;
		int m_Field0E0;
		int m_Field0E4;
		char m_Field0E8;
		int m_Field0EC;
	};
	BB_ASSERT_SIZEOF(CParticleController, 0x90);

	class CGlitterPlayer : public Hedgehog::Base::CObject
	{
	public:
		virtual ~CGlitterPlayer();

	private:
		virtual void PlayOneshotByMatrix(const Hedgehog::Math::CMatrix& in_Matrix, const Hedgehog::Base::CSharedString& in_AssetName, float in_Size, int in_ID) {}
		virtual void PlayOneshotByNode(const boost::shared_ptr<Hedgehog::Mirage::CMatrixNode>& in_MatrixNode, const Hedgehog::Base::CSharedString& in_AssetName, float in_Size, int in_ID) {}

		virtual bool PlayContinuousByMatrix(Hedgehog::Base::THolder<CGameDocument> in_Holder,
			const Hedgehog::Math::CMatrix& in_Matrix,
			const Hedgehog::Base::CSharedString& in_AssetName, float in_Size, int usuallyOne,
			int usuallyZero)
		{
			return false;
		}
		virtual bool PlayContinuousByNode(Hedgehog::Base::THolder<CGameDocument> in_Holder,
			const boost::shared_ptr<Hedgehog::Mirage::CMatrixNode>& in_spNode,
			const Hedgehog::Base::CSharedString& in_AssetName, float in_Size, int usuallyOne,
			int usuallyZero)
		{
			return false;
		}

	public:

		// I prefer function overloads personally, so I'm doing it this way.
		void PlayOneshot(const Hedgehog::Math::CMatrix& in_Matrix, const Hedgehog::Base::CSharedString& in_AssetName, float in_Size, int in_ID)
		{
			PlayOneshotByMatrix(in_Matrix, in_AssetName, in_Size, in_ID);
		}
		void PlayOneshot(const boost::shared_ptr<Hedgehog::Mirage::CMatrixNode>& in_MatrixNode, const Hedgehog::Base::CSharedString& in_AssetName, float in_Size, int in_ID)
		{
			PlayOneshotByNode(in_MatrixNode, in_AssetName, in_Size, in_ID);
		}
		bool PlayContinuous(const Hedgehog::Base::TSynchronizedPtr<Sonic::CGameDocument>& pGameDocument,
			const boost::shared_ptr<Hedgehog::Mirage::CMatrixNode>& in_spNode,
			const Hedgehog::Base::CSharedString& in_AssetName, float in_Size, int usuallyOne = 1, int usuallyZero = 0)
		{
			return PlayContinuousByNode(pGameDocument.get(), in_spNode, in_AssetName, in_Size, usuallyOne, usuallyZero);
		}
		bool PlayContinuous(const Hedgehog::Base::TSynchronizedPtr<Sonic::CGameDocument>& pGameDocument,
			const Hedgehog::Math::CMatrix& in_Matrix,
			const Hedgehog::Base::CSharedString& in_AssetName, float in_Size, int usuallyOne = 1, int usuallyZero = 0)
		{
			return PlayContinuousByMatrix(pGameDocument.get(), in_Matrix, in_AssetName, in_Size, usuallyOne, usuallyZero);
		}

		CParticleController* m_pParticleController;

		static CGlitterPlayer* Make(CGameDocument* pGameDocument)
		{
			BB_FUNCTION_PTR(CGlitterPlayer*, __cdecl, Func, 0x01255B40, Sonic::CGameDocument * in_pGameDocument);
			CGlitterPlayer* result = Func(pGameDocument);

			return result;
		}
	};
}
