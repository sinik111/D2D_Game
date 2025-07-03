# pragma once

#include <cmath>

class MyMath
{
public:
	static constexpr float EPSILON = 0.0001f;
	static constexpr float PI = 3.1415f;

	template <typename T>
	static T Clamp(T value, T min, T max)
	{
		return value < min ? min : (value > max ? max : value);
	}
};