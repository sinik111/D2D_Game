#pragma once

#include <d2d1_1.h>
#include <DirectXMath.h>
#include <cassert>

#include "Vector2.h"
#include "MyMath.h"

struct Matrix3x2
{
private: // SIMD 연산을 위한 DirectXMath의 XMMATRIX
	DirectX::XMMATRIX m_rawMatrix;

public:
	Matrix3x2()
		: m_rawMatrix{ DirectX::XMMatrixIdentity() }
	{

	}

	explicit Matrix3x2(DirectX::XMMATRIX rawMatrix)
		: m_rawMatrix{ rawMatrix }
	{

	}

	explicit Matrix3x2(const D2D1::Matrix3x2F& d2d1Matrix)
        : m_rawMatrix{
            d2d1Matrix._11, d2d1Matrix._12, 0.0f, 0.0f,
            d2d1Matrix._21, d2d1Matrix._22, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            d2d1Matrix._31, d2d1Matrix._32, 0.0f, 1.0f }
	{

	}

public: // 연산자 오버로딩
    Matrix3x2 operator*(const Matrix3x2& other) const
    {
        return Matrix3x2(DirectX::XMMatrixMultiply(m_rawMatrix, other.m_rawMatrix));
    }

public: // Get, Set, As
    D2D1_MATRIX_3X2_F AsD2D1Matrix() const
    {
        DirectX::XMVECTOR r0 = m_rawMatrix.r[0];
        DirectX::XMVECTOR r1 = m_rawMatrix.r[1];
        DirectX::XMVECTOR r3 = m_rawMatrix.r[3];

        return D2D1::Matrix3x2F(
            DirectX::XMVectorGetX(r0), DirectX::XMVectorGetY(r0),
            DirectX::XMVectorGetX(r1), DirectX::XMVectorGetY(r1),
            DirectX::XMVectorGetX(r3), DirectX::XMVectorGetY(r3)
        );
    }

    DirectX::XMMATRIX AsXMMatrix() const
    {
        return m_rawMatrix;
    }

public: // 유틸리티 함수
    static Matrix3x2 Translation(float x, float y)
    {
        return Matrix3x2(DirectX::XMMatrixTranslation(x, y, 0.0f));
    }

    static Matrix3x2 Translation(Vector2 position)
    {
        return Matrix3x2(DirectX::XMMatrixTranslation(position.GetX(), position.GetY(), 0.0f));
    }

    static Matrix3x2 Rotation(float angleDegrees)
    {
        return Matrix3x2(DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(angleDegrees)));
    }

    static Matrix3x2 Scale(float sx, float sy)
    {
        return Matrix3x2(DirectX::XMMatrixScaling(sx, sy, 1.0f));
    }

    static Matrix3x2 Scale(Vector2 scale)
    {
        return Matrix3x2(DirectX::XMMatrixScaling(scale.GetX(), scale.GetY(), 1.0f));
    }

    Matrix3x2 Inverse() const
    {
        DirectX::XMVECTOR determinant;

        return Matrix3x2(DirectX::XMMatrixInverse(&determinant, m_rawMatrix));
    }
};