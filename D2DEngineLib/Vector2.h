#pragma once

#include <d2d1_1.h>
#include <cassert>
#include <sstream>

#include "MyMath.h"

struct Vector2 : 
	public D2D1_VECTOR_2F
{
public:
	Vector2()
		: D2D1_VECTOR_2F{}
	{

	}

	Vector2(float x, float y)
		: D2D1_VECTOR_2F{ x, y }
	{

	}

public: // 연산자 오버로딩
	Vector2 operator+(const Vector2& rhs) const
	{
		return Vector2(x + rhs.x, y + rhs.y);
	}
	
	Vector2 operator-(const Vector2& rhs) const
	{
		return Vector2(x - rhs.x, y - rhs.y);
	}

	Vector2 operator-() const
	{
		return Vector2(-x, -y);
	}
	
	Vector2 operator*(float scalar) const
	{
		return Vector2(x * scalar, y * scalar);
	}
	
	friend inline Vector2 operator*(float scalar, const Vector2& v);
	
	Vector2 operator/(float scalar) const
	{
		if (std::abs(scalar) <= MyMath::EPSILON)
		{
			assert(false && "0으로 나누고 있습니다");
			return Vector2(INFINITY, INFINITY);
		}
	
		return Vector2(x / scalar, y / scalar);
	}
	
	Vector2& operator+=(const Vector2& rhs)
	{
		x += rhs.x;
		y += rhs.y;
	
		return *this;
	}
	
	Vector2& operator-=(const Vector2& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
	
		return *this;
	}
	
	Vector2& operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;

		return *this;
	}
	
	Vector2& operator/=(float scalar)
	{
		if (std::abs(scalar) <= MyMath::EPSILON)
		{
			assert(false && "0으로 나누고 있습니다");

			x = INFINITY;
			y = INFINITY;
	
			return *this;
		}
	
		x /= scalar;
		y /= scalar;
	
		return *this;
	}

	//비교연산자 추가 오버로딩. 
	bool operator==(const Vector2& rhs) const
	{
		return std::abs(x - rhs.x) <= MyMath::EPSILON &&
			std::abs(y - rhs.y) <= MyMath::EPSILON;
	}

	
	bool operator!=(const Vector2& rhs) const
	{
		return !(*this == rhs); // == 연산자를 활용하여 구현
	}
	
public: // 유틸리티 함수
	Vector2 Normalized() const
	{
		float length = std::sqrt(x * x + y * y);

		if (std::abs(length) <= MyMath::EPSILON)
		{
			assert(false && "0으로 나누고 있습니다");

			return Vector2(INFINITY, INFINITY);
		}

		return Vector2(x / length, y / length);
	}
	
	void Normalize()
	{
		float length = std::sqrt(x * x + y * y);

		if (std::abs(length) <= MyMath::EPSILON)
		{
			assert(false && "0으로 나누고 있습니다");

			x = INFINITY;
			y = INFINITY;

			return;
		}

		x /= length;
		y /= length;
	}
	
	float Length() const
	{
	    return std::sqrt(x * x + y * y);
	}
	
	float LengthSq() const
	{
	    return x * x + y * y;
	}
	
	static float Dot(const Vector2& v1, const Vector2& v2)
	{
	    return v1.x * v2.x + v1.y * v2.y;
	}

	static float Cross(const Vector2& v1, const Vector2& v2)
	{
		return v1.x * v2.y - v1.y * v2.x;
	}
	
	static float Distance(const Vector2& v1, const Vector2& v2)
	{
	    return (v1 - v2).Length();
	}
	
	static float DistanceSq(const Vector2& v1, const Vector2& v2)
	{
		return (v1 - v2).LengthSq();
	}
	
	static Vector2 Direction(const Vector2& to, const Vector2& from)
	{
		return (to - from).Normalized();
	}
	
	static Vector2 Lerp(const Vector2& v0, const Vector2& v1, float t)
	{
		t = MyMath::Clamp(t, 0.0f, 1.0f);
			
		return Vector2(v0.x * (1.0f - t) + v1.x * t, v0.y * (1.0f - t) + v1.y * t);
	}
	
	static Vector2 Reflect(const Vector2& dir, const Vector2& normal)
	{
		return dir - normal * 2 * Dot(dir, normal);
	}

	static float LerpRatio(const Vector2& v0, const Vector2& v1, const Vector2& current)
	{
		Vector2 v01 = v1 - v0;
		Vector2 v0c = current - v0;

		float squareLength = v01.LengthSq();
		if (squareLength < MyMath::EPSILON)
		{
			assert(false && "0으로 나누고 있습니다");
			return 0.0f;
		}

		return Dot(v01, v0c) / squareLength;
	}

	static Vector2 Abs(const Vector2& v)
	{
		return Vector2(std::abs(v.x), std::abs(v.y));
	}

	static Vector2 RotateVector(Vector2 v, float degree)
	{
		float radian = degree * MyMath::DegToRad;

		Vector2 rotated;
		rotated.x = v.x * std::cos(radian) - v.y * std::sin(radian);
		rotated.y = v.x * std::sin(radian) + v.y * std::cos(radian);
		return rotated;
	}

	std::string ToString() const;
	
public:
	static const Vector2 Zero;
	static const Vector2 One;
	static const Vector2 Up;
	static const Vector2 Down;
	static const Vector2 Right;
	static const Vector2 Left;

	static const Vector2 EllipseRightUp;
	static const Vector2 EllipseLeftUp;
	static const Vector2 EllipseRightDown;
	static const Vector2 EllipseLeftDown;
};
	
inline Vector2 operator*(float scalar, const Vector2& v)
{
	return Vector2(v.x * scalar, v.y * scalar);
}

inline std::wostringstream& operator<<(std::wostringstream& woss, const Vector2& v)
{
	woss << L"{ " << v.x << L", " << v.y << L" }";

	return woss;
}