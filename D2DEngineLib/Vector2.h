#pragma once

#include <d2d1_1.h>
#include <DirectXMath.h>
#include <cassert>

#include "MyMath.h"

struct Vector2
{
private: // SIMD 연산을 위한 DirectXMath의 XMVECTOR
	DirectX::XMVECTOR m_rawVector;

public: // 생성자
	Vector2()
		: m_rawVector{ DirectX::XMVectorZero() }
	{

	}

	Vector2(float x, float y)
		: m_rawVector{ DirectX::XMVectorSet(x, y, 0.0f, 0.0f) }
	{

	}

	explicit Vector2(DirectX::XMVECTOR rawVector)
		: m_rawVector{ rawVector }
	{

	}

	explicit Vector2(D2D1_VECTOR_2F d2d1Vector2)
		: m_rawVector{ DirectX::XMVectorSet(d2d1Vector2.x, d2d1Vector2.y, 0.0f, 0.0f) }
	{
		
	}

public: // 연산자 오버로딩
	Vector2 operator+(const Vector2& other) const
	{
		return Vector2(DirectX::XMVectorAdd(m_rawVector, other.m_rawVector));
	}

	Vector2 operator-(const Vector2& other) const
	{
		return Vector2(DirectX::XMVectorSubtract(m_rawVector, other.m_rawVector));
	}

	Vector2 operator*(float scalar) const
	{
		return Vector2(DirectX::XMVectorScale(m_rawVector, scalar));
	}

	friend inline Vector2 operator*(float scalar, const Vector2& v);

	Vector2 operator/(float scalar) const
	{
		assert(std::fabs(scalar) > MyMath::EPSILON && "0으로 나누고 있습니다");

		if (std::fabs(scalar) <= MyMath::EPSILON)
		{
			return Vector2(INFINITY, INFINITY);
		}

		return Vector2(DirectX::XMVectorScale(m_rawVector, 1 / scalar));
	}

	Vector2& operator+=(const Vector2& other)
	{
		m_rawVector = DirectX::XMVectorAdd(m_rawVector, other.m_rawVector);

		return *this;
	}

	Vector2& operator-=(const Vector2& other)
	{
		m_rawVector = DirectX::XMVectorSubtract(m_rawVector, other.m_rawVector);

		return *this;
	}

	Vector2& operator*=(float scalar)
	{
		m_rawVector = DirectX::XMVectorScale(m_rawVector, scalar);

		return *this;
	}

	Vector2& operator/=(float scalar)
	{
		assert(std::fabs(scalar) > MyMath::EPSILON && "0으로 나누고 있습니다");

		if (std::fabs(scalar) <= MyMath::EPSILON)
		{
			m_rawVector = DirectX::XMVectorSet(INFINITY, INFINITY, 0.0f, 0.0f);

			return *this;
		}

		m_rawVector = DirectX::XMVectorScale(m_rawVector, 1 / scalar);

		return *this;
	}

public: // Get, Set, As
	float GetX() const
	{
		return DirectX::XMVectorGetX(m_rawVector);
	}

	float GetY() const
	{
		return DirectX::XMVectorGetY(m_rawVector);
	}

	void SetX(float x)
	{
		m_rawVector = DirectX::XMVectorSetX(m_rawVector, x);
	}
	
	void SetY(float y)
	{
		m_rawVector = DirectX::XMVectorSetY(m_rawVector, y);
	}

	D2D1_VECTOR_2F AsD2D1Vector2F() const
	{
		return { DirectX::XMVectorGetX(m_rawVector), DirectX::XMVectorGetY(m_rawVector) };
	}

	DirectX::XMVECTOR AsXMVector() const
	{
		return m_rawVector;
	}

public: // 유틸리티 함수
    Vector2 Normalized() const
	{
		return Vector2(DirectX::XMVector2Normalize(m_rawVector));
    }

	void Normalize()
	{
		m_rawVector = DirectX::XMVector2Normalize(m_rawVector);
	}

    float Length() const
	{
        return DirectX::XMVectorGetX(DirectX::XMVector2Length(m_rawVector));
    }

    float LengthSq() const
	{
        return DirectX::XMVectorGetX(DirectX::XMVector2LengthSq(m_rawVector));
    }

    static float Dot(const Vector2& v1, const Vector2& v2)
	{
        return DirectX::XMVectorGetX(DirectX::XMVector2Dot(v1.m_rawVector, v2.m_rawVector));
    }

    static float Distance(const Vector2& v1, const Vector2& v2)
	{
        return DirectX::XMVectorGetX(DirectX::XMVector2Length(DirectX::XMVectorSubtract(v1.m_rawVector, v2.m_rawVector)));
    }

    static float DistanceSq(const Vector2& v1, const Vector2& v2)
	{
        return DirectX::XMVectorGetX(DirectX::XMVector2LengthSq(DirectX::XMVectorSubtract(v1.m_rawVector, v2.m_rawVector)));
    }

	static Vector2 Direction(const Vector2& to, const Vector2& from)
	{
		return Vector2(DirectX::XMVector2Normalize(DirectX::XMVectorSubtract(to.m_rawVector, from.m_rawVector)));
	}

	static Vector2 Lerp(const Vector2& v0, const Vector2& v1, float t)
	{
		t = MyMath::Clamp(t, 0.0f, 1.0f);

		return Vector2(DirectX::XMVectorLerp(v0.m_rawVector, v1.m_rawVector, t));
	}

	static Vector2 Reflect(const Vector2& dir, const Vector2& normal)
	{
		return Vector2(DirectX::XMVector2Reflect(dir.m_rawVector, normal.m_rawVector));
	}

public:
	static const Vector2 Zero;
	static const Vector2 One;
	static const Vector2 Up;
	static const Vector2 Down;
	static const Vector2 Right;
	static const Vector2 Left;
};

inline Vector2 operator*(float scalar, const Vector2& v)
{
	return Vector2(DirectX::XMVectorScale(v.m_rawVector, scalar));
}