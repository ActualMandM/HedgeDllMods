#pragma once
#include "hhMath.h"

class MathUtil
{
public:

	static inline float RadiansToDegrees = 57.29578018188477f; // 57.295779513f;
	static inline float DegreesToRadians = 0.01745329238474369f;

	// Reference: 0x006F01F0
	static Hedgehog::Math::CVector ConvertToSpherePolar(const Hedgehog::Math::CVector& in_rVector)
	{
		const float HalfPiZ = M_PI * 0.5 + in_rVector.z();
		const float sin_HalfPiZ = sin(HalfPiZ);

		const float sin_Y = sin(in_rVector.y());
		const float cos_Y = cos(in_rVector.y());

		const float cos_HalfPiZ = cos(HalfPiZ);

		const Hedgehog::Math::CVector result(sin_Y * in_rVector.x() * sin_HalfPiZ,
			in_rVector.x() * cos_HalfPiZ,
			cos_Y * in_rVector.x() * sin_HalfPiZ);

		return result;
	}

	// Reference: 0x006F2550
	static void AngleWrap(float* a1, float a2)
	{
		if (*a1 - a2 <= M_PI)
		{
			if (a2 - *a1 > M_PI)
				*a1 += (M_PI * 2.0);
		}
		else
		{
			*a1 -= (M_PI * 2.0);
		}
	}

	// Reference: 0x6F24E0
	static void AdjustAngleGivenSpeedAndTime(float* currentAngle, const float targetAngle, const float speed, const float deltaTime)
	{
		const float numberMinusArg1 = *currentAngle - targetAngle;
		const float currentSpeed = speed * deltaTime;
		if (*currentAngle <= targetAngle)
		{
			if (currentSpeed < -numberMinusArg1)
			{
				*currentAngle = currentSpeed + *currentAngle;
				return;
			}
		}
		else if (currentSpeed < numberMinusArg1)
		{
			*currentAngle = *currentAngle - currentSpeed;
			return;
		}
		*currentAngle = targetAngle;
	}

	// Location: 0x6F25E0
	static void WrapFloatGivenCrap(float* currentAngle, float a2, float a3, float minAngle, float maxAngle, float a6, float deltaTime)
	{
		float v14 = (a3 - a2) / minAngle * maxAngle;
		const float v11 = fabs(v14);
		const double v8 = v11;
		const float v12 = fabs(maxAngle);
		if (v12 < v8)
		{
			v14 = v14 <= 0.0 ? -maxAngle : maxAngle;
		}
		AdjustAngleGivenSpeedAndTime(currentAngle, v14, a6, deltaTime);
		const float v13 = fabs(*currentAngle);
		const double v10 = v14;
		const float v15 = fabs(v14);
		if (v15 < v13)
			*currentAngle = v10;
	}

	enum class AxisType
	{
		XZ,
		XY,
		ZY,
	};
	static float GetPlanarMagnitude(const Hedgehog::Math::CVector& vec, AxisType index = AxisType::XZ)
	{
		float sqrMag;
		switch (index)
		{
		default:
		case AxisType::XZ:
			sqrMag = vec.x() * vec.x() + vec.z() * vec.z();
			break;
		case AxisType::XY:
			sqrMag = vec.x() * vec.x() + vec.y() * vec.y();
			break;
		case AxisType::ZY:
			sqrMag = vec.z() * vec.z() + vec.y() * vec.y();
			break;
		}
		return std::sqrt(sqrMag);
	}

	static inline float Sign(const float num)
	{
		return num > 0 ? 1.0f : -1.0f;
	}

	static inline float InverseLerpUnclamped(const float low, const float high, const float value)
	{
		const float div = high - low;
		if (std::abs(div) < std::numeric_limits<float>::epsilon()) return 0;
		return (value - low) / div;
	}

	static inline float InverseLerp(const float low, const float high, const float value)
	{
		float result = InverseLerpUnclamped(low, high, value);
		if (result > 1.0f) result = 1.0f;
		if (result < 0.0f) result = 0.0f;
		return result;
	}

	static inline float QuickPower(const float num, const int exp)
	{
		float result = 1.0f;
		float thisNum = num;
		int thisExp = exp;
		while (thisExp > 0)
		{
			if (thisExp % 2 == 1) result *= thisNum;
			thisExp >>= 1;
			thisNum *= thisNum;
		}
		return result;
	}

	static inline float ReversePower(const float value, const float exp)
	{
		const float result = 1 - std::pow(1 - value, exp);
		return result;
	}

	static inline float ReversePower(const float value, const int exp)
	{
		const bool isNegative = value < 0;
		const float _value = isNegative ? -value : value;

		const float result = 1 - QuickPower(1 - _value, exp);
		return isNegative ? -result : result;
	}

#if 0
	// Reference: 0x006F2180
	static Hedgehog::Math::CVector MatrixTransformVector(const Hedgehog::Math::CMatrix44& in_rMatrix, const Hedgehog::Math::CVector& in_rBaseVector)
	{
		using namespace Hedgehog::Math;

		const float f1  = in_rMatrix.GetFloatFromMatrix(0, 1) * in_rBaseVector.y();
		const float f2  = in_rMatrix.GetFloatFromMatrix(0, 0) * in_rBaseVector.x() + f1;
		const float f3  = in_rMatrix.GetFloatFromMatrix(0, 2) * in_rBaseVector.z() + f2;
		const float f4  = in_rMatrix.GetFloatFromMatrix(0, 3) + f3;
		const float f5  = in_rMatrix.GetFloatFromMatrix(1, 1) * in_rBaseVector.y();
		const float f6  = in_rMatrix.GetFloatFromMatrix(1, 0) * in_rBaseVector.x() + f5;
		const float f7  = in_rMatrix.GetFloatFromMatrix(1, 2) * in_rBaseVector.z() + f6;
		const float f8  = in_rMatrix.GetFloatFromMatrix(1, 3) + f7;
		const float f9  = in_rMatrix.GetFloatFromMatrix(2, 1) * in_rBaseVector.y();
		const float f10 = in_rMatrix.GetFloatFromMatrix(2, 0) * in_rBaseVector.x() + f9;
		const float f11 = in_rMatrix.GetFloatFromMatrix(2, 2) * in_rBaseVector.z() + f10;
		const float f12 = in_rMatrix.GetFloatFromMatrix(2, 3) + f11;
		const float f13 = in_rMatrix.GetFloatFromMatrix(3, 1) * in_rBaseVector.y();
		const float f14 = in_rMatrix.GetFloatFromMatrix(3, 0) * in_rBaseVector.x() + f13;
		const float f15 = in_rMatrix.GetFloatFromMatrix(3, 2) * in_rBaseVector.z() + f14;
		const float f16 = in_rMatrix.GetFloatFromMatrix(3, 3);

		const float calc = 1.0 / fabs(f16 + f15);
		CVector out(calc * f4, calc * f8, calc * f12);
		return out;
	};
#endif
};
