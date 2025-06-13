# pragma once

#include <d2d1_1.h>

class MyMath
{
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

inline D2D1_VECTOR_2F operator+(const D2D1_VECTOR_2F& v1, const D2D1_VECTOR_2F& v2)
{
	return { v1.x + v2.x, v1.y + v2.y };
}

inline D2D1_VECTOR_2F operator-(const D2D1_VECTOR_2F& v1, const D2D1_VECTOR_2F& v2)
{
	return { v1.x - v2.x, v1.y - v2.y };
}

inline D2D1_VECTOR_2F operator*(const D2D1_VECTOR_2F& v, float s)
{
	return { v.x * s, v.y * s };
}

inline D2D1_VECTOR_2F operator/(const D2D1_VECTOR_2F& v, float s)
{
	return { v.x / s, v.y / s };
}

inline D2D1_VECTOR_2F& operator+=(D2D1_VECTOR_2F& v1, const D2D1_VECTOR_2F& v2)
{
	v1.x += v2.x;
	v1.y += v2.y;

	return v1;
}

inline D2D1_VECTOR_2F& operator-=(D2D1_VECTOR_2F& v1, const D2D1_VECTOR_2F& v2)
{
	v1.x += v2.x;
	v1.y += v2.y;

	return v1;
}

inline D2D1_VECTOR_2F& operator*=(D2D1_VECTOR_2F& v, float s)
{
	v.x *= s;
	v.y *= s;

	return v;
}

inline D2D1_VECTOR_2F& operator/=(D2D1_VECTOR_2F& v, float s)
{
	v.x /= s;
	v.y /= s;

	return v;
}