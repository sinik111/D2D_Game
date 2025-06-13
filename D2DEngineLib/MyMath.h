# pragma once

#include <cmath>

class MyMath
{
public:
	static constexpr float EPSILON = 0.0001f;
	static constexpr float PI = 3.1415f;

	template <typename T>
	static T Clamp(T value, T minVal, T maxVal)
	{
		return (value < minVal) ? minVal : (value > maxVal) ? maxVal : value;
	}

	template <typename T>
	static T Square(T value)
	{
		return value * value;
	}
};