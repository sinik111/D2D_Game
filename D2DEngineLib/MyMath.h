# pragma once

#include <cmath>

class MyMath
{
public:
	static constexpr float EPSILON = 0.0001f;
	static constexpr float PI = 3.1415926535f;
	static constexpr float RadToDeg = 180.0f / PI;
	static constexpr float DegToRad = PI / 180.0f;

	template <typename T>
	static T Clamp(T value, T min, T max)
	{
		return value < min ? min : (value > max ? max : value);
	}

	static float Lerp(float f0, float f1, float t)
	{
		t = t < 0.0f ? 0.0f : (t > 1.0f ? 1.0f : t);

		return (1.0f - t) * f0 + t * f1;
	}

	static float NormalizeRadian(float radian)
	{
		radian = std::fmodf(radian, PI * 2.0f);
		if (radian < 0.0f)
		{
			radian += PI * 2.0f;
		}

		return radian;
	}

	static float NormalizeDegree(float degree)
	{
		degree = std::fmodf(degree, 360.0f);
		if (degree < 0.0f)
		{
			degree += 360.0f;
		}

		return degree;
	}
};