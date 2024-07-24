#pragma once
namespace Hedgehog::Glitter
{
	class CAnimationCurve;
	class CMaterial;
	class CRenderGroup;

	class EmitterParameter;

	struct SUVData
	{
		float U;
		float V;
	};
	struct SUVData16
	{
		uint16_t U;
		uint16_t V;
	};
	struct ColorBGRA
	{
		uint8_t b;
		uint8_t g;
		uint8_t r;
		uint8_t a;
	};

	// TODO: make not bad
	enum EEffectParameterFlags
	{
		EffectFLAG_0 = 0x0,
		EffectFLAG_1 = 0x1,
		EffectFLAG_2 = 0x2,
		EffectFLAG_4 = 0x4,
		EffectFLAG_8 = 0x8,
		EffectFLAG_16 = 0x10,
		EffectFLAG_32 = 0x20,
		EffectFLAG_64 = 0x40,
		EffectFLAG_128 = 0x80,
		EffectFLAG_256 = 0x100,
		EffectFLAG_512 = 0x200,
		EffectFLAG_1024 = 0x400,
		EffectFLAG_2048 = 0x800,
		EffectFLAG_4096 = 0x1000,
		EffectFLAG_8192 = 0x2000,
		EffectFLAG_16384 = 0x4000,
		EffectFLAG_32768 = 0x8000,
		EffectFLAG_65536 = 0x10000,
		EffectFLAG_131072 = 0x20000,
		EffectFLAG_262144 = 0x40000,
		EffectFLAG_524288 = 0x80000,
		EffectFLAG_1048576 = 0x100000,
		EffectFLAG_2097152 = 0x200000,
		EffectFLAG_4194304 = 0x400000,
		EffectFLAG_8388608 = 0x800000,
		EffectFLAG_16777216 = 0x1000000,
		EffectFLAG_33554432 = 0x2000000,
		EffectFLAG_67108864 = 0x4000000,
		EffectFLAG_134217728 = 0x8000000,
		EffectFLAG_268435456 = 0x10000000,
		EffectFLAG_536870912 = 0x20000000,
		EffectFLAG_1073741824 = 0x40000000,
		EffectFLAG_2147483648 = 0x80000000,
	};
	enum ETypeEmissionDirectionType
	{
		ParticleVelocity = 0x0,
		Outward = 0x1,
		Inward = 0x2,
	};
	enum ETypeDirectionType
	{
		ParentAxis = 0x0,
		Billboard = 0x1,
		XAxis = 0x2,
		YAxis = 0x3,
		ZAxis = 0x4,
		YRotationOnly = 0x5,
	};
	enum ETypeEmitCondition
	{
		Time = 0x0,
		MovingDistance = 0x1,
	};
	enum ETypeEmitter
	{
		Box = 0x0,
		Cylinder = 0x1,
		Sphere = 0x2,
		Polygon = 0x4,
	};

	struct __declspec(align(4)) ParticleParameter
	{
		enum EAddressMode
		{
			Clamp = 0x0,
			Wrap = 0x1,
		};

		enum EBlendMode
		{
			Zero = 0x7,
			Typical = 0x1,
			Add = 0x2,
			Subtract = 0x3,
			Multiply = 0x4,
			PunchThrough = 0x5,
			Opaque = 0x6,
			UseMaterial = 0x7,
		};

		enum EDirectionType
		{
			DirectionalAngle = 0x1,
			DirectionalAngleBillboard = 0x2,
			EmitterDirection = 0x3,
			EmitterAxis = 0x4,
			EmittedEmitterAxis = 0x5,
		};

		enum EPivotPosition
		{
			TopLeft = 0x0,
			TopCenter = 0x1,
			TopRight = 0x2,
			MiddleLeft = 0x3,
			MiddleCenter = 0x4,
			MiddleRight = 0x5,
			BottomLeft = 0x6,
			BottomCenter = 0x7,
			BottomRight = 0x8,
		};

		enum EType
		{
			Quad = 0x0,
			Mesh = 0x1,
			Locus = 0x2,
			Line = 0x3,
		};

		enum EUVIndexType
		{
			Fixed = 0x0,
			InitialRandom = 0x1,
			RandomOrder = 0x2,
			SequentialOrder = 0x3,
			ReverseOrder = 0x4,
			InitialRandomSequentialOrder = 0x5,
			InitialRandomReverseOrder = 0x6,
			User = 0x7,
		};

		Base::CSharedString Name;
		EType Type;
		float LifeTime;
		EPivotPosition PivotPosition;
		EDirectionType DirectionType;
		float ZOffset;
		Math::CVector Size;
		Math::CVector SizeRandom;
		Math::CVector Rotation;
		Math::CVector RotationRandom;
		Math::CVector RotationAdd;
		Math::CVector RotationAddRandom;
		Math::CVector Direction;
		Math::CVector DirectionRandom;
		float Speed;
		float SpeedRandom;
		Math::CVector GravitationalAccel;
		Math::CVector ExternalAccel;
		Math::CVector ExternalAccelRandom;
		float Deceleration;
		float DecelerationRandom;
		float EmitterTranslationEffectRatio;
		float FollowEmitterTranslationRatio;
		float FollowEmitterTranslationYRatio;
		float ReflectionCoeff;
		float ReflectionCoeffRandom;
		float ReboundPlaneY;
		boost::shared_ptr<EmitterParameter> m_spEmitterParameter;
		float ChildEmitterTime;
		int MaxCount;
		ColorBGRA Color8Bit;
		int TextureIndex;
		EUVIndexType UvIndexType;
		int UvIndex;
		int UvIndexStart;
		int UvIndexEnd;
		int UvChangeInterval;
		SUVData ColorScroll;
		SUVData ColorScrollRandom;
		float ColorScrollSpeed;
		SUVData AlphaScroll;
		SUVData AlphaScrollRandom;
		float AlphaScrollSpeed;
		SUVData SecondaryColorScroll;
		SUVData SecondaryColorScrollRandom;
		float SecondaryColorScrollSpeed;
		SUVData SecondaryAlphaScroll;
		SUVData SecondaryAlphaScrollRandom;
		float SecondaryAlphaScrollSpeed;
		enum EParticleParameterFlags
		{
			// Fill these out when i care
		} Flags;
		hh::vector<CAnimationCurve> m_AnimationCurveList;
		Base::CSharedString Material;
		boost::shared_ptr<CMaterial> m_spGlitterMaterial;
		EBlendMode BlendMode;
		EBlendMode SecondaryBlendMode;
		int SecondaryBlend;
		EAddressMode AddressMode;
		Base::CSharedString MeshName;
		int m_Field1B0;
		int m_Field1B4;
		int LocusHistorySize;
		int LocusHistorySizeRandom;
	};

	struct __declspec(align(4)) EmitterParameter
	{
		Base::CSharedString m_Name;
		ETypeEmitter Type;
		float StartTime;
		float LifeTime;
		float LoopStartTime;
		float LoopEndTime;
		Math::CVector Translation;
		Math::CVector Rotation;
		Math::CVector RotationAdd;
		Math::CVector RotationAddRandom;
		Math::CVector Scaling;
		ETypeDirectionType DirectionType;
		ETypeEmitCondition EmitCondition;
		float EmissionInterval;
		float ParticlePerEmission;
		int Flags;
		hh::vector<CAnimationCurve> m_AnimationCurves;
		hh::vector<ParticleParameter> m_ParticleParamsVector;
		int m_Field0A4;
		int m_Field0A8;
		int m_Field0AC;
		Math::CVector Box_Size;
		int Cylinder_Radius;
		int Cylinder_Height;
		float Cylinder_StartAngle;
		float Cylinder_EndAngle;
		ETypeEmissionDirectionType Cylinder_EmissionDirectionType;
		float Sphere_Radius;
		float Sphere_Latitude;
		float Sphere_Longitude;
		ETypeEmissionDirectionType Sphere_EmissionDirectionType;
		Base::CSharedString Mesh_Name;
		float Polygon_Radius;
		int Polygon_PointCount;
	};

	struct __declspec(align(4)) EffectParameter
	{
		Base::CSharedString Name;
		float StartTime;
		float LifeTime;
		ColorBGRA m_Color8Bit;
		Math::CVector Translation;
		Math::CVector Rotation;
		EEffectParameterFlags Flags;
		hh::vector<boost::shared_ptr<EmitterParameter>> m_Vector34;
		hh::vector<boost::shared_ptr<EmitterParameter>> m_EmitterParameters;
		int m_Field054;
		int m_Field058;
		int m_Field05C;
	};

	// -------------------------------------------------------

	class CParticle : public Base::CObject
	{
	public:
		Base::CSharedString m_Text004;
		Base::CSharedString m_Text008;
		int m_Field00C;
		boost::shared_ptr<ParticleParameter> m_spParticleParameter;
		int m_Field018;
		boost::shared_ptr<CMaterial> m_spGlitterMaterial;
		boost::shared_ptr<CRenderGroup> m_spGlitterRenderGroup;
		int m_Field02C;
		Math::CVector m_Position;
		Math::CMatrix44 m_EmitterMatrix;
		Math::CMatrix44 m_Field080;
		Math::CVector m_Scale;
		Math::CVector m_Field0D0;
		int m_Field0E0;
		int m_Field0E4;
		int m_Field0E8;
		int m_Field0EC;
	};

	class CEmitter : public Base::CObject
	{
	public:
		int m_Field004;
		int m_Field008;
		int m_Field00C;
		boost::shared_ptr<EmitterParameter> m_spEmitterParameter;
		hh::list<boost::shared_ptr<CParticle>> m_ParticleList;
		int m_Field024;
		int m_Field028;
		int m_Field02C;
		Math::CVector Translation;
		Math::CVector Rotation;
		Math::CVector ScaleBase;
		Math::CVector Scale;
		float ScaleAll;
		float EmissionInterval;
		float ParticlePerEmission;
		float m_EmissionStrengthPossibly;
		SUVData16 m_UVData;
		int m_Field084;
		int m_Field088;
		int m_Field08C;
		Math::CVector m_PinnedPosition;
		Math::CMatrix44 m_EmitterMatrix;
		Math::CMatrix44 m_Field0E0;
		Math::CVector m_EmitterPosition;
		Math::CVector m_Field130;
		float MovingDistanceParam;
		float m_ParticleRemainder;
		float CurrentTime;
		//todo: make good
		enum EParticleFlags
		{
			ParticleFLAG_1 = 0x1,
			ParticleFLAG_2 = 0x2,
			ParticleFLAG_3 = 0x4,
			ParticleFLAG_4 = 0x8,
			ParticleFLAG_5 = 0x10,
			ParticleFLAG_6 = 0x20,
			ParticleFLAG_7 = 0x40,
			ParticleFLAG_8 = 0x80,
			ParticleFLAG_9 = 0x100,
			ParticleFLAG_10 = 0x200,
			ParticleFLAG_11 = 0x400,
			ParticleFLAG_12 = 0x800,
			ParticleFLAG_13 = 0x1000,
			ParticleFLAG_14 = 0x2000,
			ParticleFLAG_15 = 0x4000,
			ParticleFLAG_16 = 0x8000,
			ParticleFLAG_17 = 0x10000,
			ParticleFLAG_18 = 0x20000,
			ParticleFLAG_19 = 0x40000,
			ParticleFLAG_20 = 0x80000,
			ParticleFLAG_21 = 0x100000,
			ParticleFLAG_22 = 0x200000,
			ParticleFLAG_23 = 0x400000,
			ParticleFLAG_24 = 0x800000,
			ParticleFLAG_25 = 0x1000000,
			ParticleFLAG_26 = 0x2000000,
			ParticleFLAG_27 = 0x4000000,
			ParticleFLAG_28 = 0x8000000,
			ParticleFLAG_29 = 0x10000000,
			ParticleFLAG_30 = 0x20000000,
			ParticleFLAG_31 = 0x40000000,
			ParticleFLAG_32 = 0x80000000,
		} m_Flags;

	};

	class CEffect : public Base::CObject
	{
	public:
		int m_Field004;
		int m_Field008;
		int m_Field00C;
		boost::shared_ptr<void> m_spEffectParameter;
		hh::list<boost::shared_ptr<CEmitter>> m_EmitterList;
		uint32_t m_ColorBGRA8; // type ColorBGRA, i think BB has one of these but idk. Imgui style maybe.
		int m_Field028;
		int m_Field02C;
		Math::CVector m_Translation;
		Math::CVector m_Rotation;
		Math::CVector m_ScaleMaybe;
		float m_ScaleFactorMaybe;
		SUVData16 UVData16;
		int m_Field068;
		int m_Field06C;
		Math::CMatrix44 m_EffectMatrix;
		float m_CurrentTime;
		int m_Field0B4;
		int m_Field0B8;
		int m_Field0BC;
	};


	class CHandle : public Base::CObject
	{
	public:
		virtual ~CHandle();
		hh::list<boost::shared_ptr<CEffect>> m_EffectList;
		int m_Field01C;
		int m_Field020;
		int m_Field024;
		int m_Field028;
		int m_Field02C;
		Math::CMatrix44 m_MatrixInternal;
		boost::shared_ptr<Mirage::CMatrixNode> m_spMatrixNode;
		int m_Field078;
		int m_Field07C;
		Math::CVector m_ParticleScale;
		Math::CQuaternion m_Quaternion90;
		Base::CSharedString m_Field0A0;
		boost::shared_ptr<Mirage::CLightFieldManager> m_spLightFieldManager;
		int m_Field0AC;
		int m_Field0B0;
		float m_Field0B4;
		int m_RenderEnableIndex;
		char m_IgnoreRotationMaybe;
		char field_BD;
		char field_BE;
		char field_BF;
		char m_Field0C0;
		char field_C1;
		char field_C2;
		char field_C3;
		char m_Field0C4;
		char field_C5;
		char field_C6;
		char field_C7;
		char m_Field0C8;
		char field_C9;
		char field_CA;
		char field_CB;
		int m_Field0CC;
	};
	BB_ASSERT_OFFSETOF(CHandle, m_EffectList, 0x10);

	class CManager;
}