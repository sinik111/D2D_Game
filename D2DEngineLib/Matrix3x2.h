#pragma once

#include <d2d1_1.h>
#include <DirectXMath.h>

#include "Vector2.h"

struct Matrix3x2 // DirectX::XMMATRIX Wrapper struct
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
            0.0f          , 0.0f          , 1.0f, 0.0f,
            d2d1Matrix._31, d2d1Matrix._32, 0.0f, 1.0f }
	{

	}

public: // 연산자 오버로딩
    Matrix3x2 operator*(const Matrix3x2& other) const
    {
        return Matrix3x2(DirectX::XMMatrixMultiply(m_rawMatrix, other.m_rawMatrix));
    }

    Matrix3x2& operator*=(const Matrix3x2& other)
    {
        m_rawMatrix = DirectX::XMMatrixMultiply(m_rawMatrix, other.m_rawMatrix);

        return *this;
    }

public: // Get, Set, As
    Vector2 GetPosition() const
    {
        return Vector2(m_rawMatrix.r[3]);
    }

    Vector2 GetScale() const
    {
        DirectX::XMVECTOR scale;
        DirectX::XMVECTOR r;
        DirectX::XMVECTOR p;

        // 행렬 분해
        DirectX::XMMatrixDecompose(&scale, &r, &p, m_rawMatrix);

        return Vector2(scale);
    }

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

    explicit operator D2D1_MATRIX_3X2_F() const
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

    static Matrix3x2 Identity()
    {
        return Matrix3x2(DirectX::XMMatrixIdentity());
    }

    void ResetScale(float scaleX = 1.0f, float scaleY = 1.0f)
    {
        // 현재 행렬에서 스케일, 회전, 변환을 분리합니다.
        DirectX::XMVECTOR scale;
        DirectX::XMVECTOR rotation;
        DirectX::XMVECTOR translation;
        DirectX::XMMatrixDecompose(&scale, &rotation, &translation, m_rawMatrix);

        // 스케일을 (1, 1, 1)로 재구성합니다 (Z는 2D 행렬에서 항상 1.0f).
        DirectX::XMVECTOR newScale = DirectX::XMVectorSet(scaleX, scaleY, 1.0f, 0.0f); // 0.0f for W

        // 새로운 스케일, 기존 회전, 기존 변환을 사용하여 행렬을 재조립합니다.
        m_rawMatrix = DirectX::XMMatrixTransformation(
            DirectX::g_XMIdentityR0, // ScaleOrigin (사용하지 않음)
            DirectX::g_XMIdentityR1, // ScaleRotation (사용하지 않음)
            newScale,                // New Scale
            DirectX::g_XMIdentityR2, // RotationOrigin (사용하지 않음)
            rotation,                // Existing Rotation
            translation              // Existing Translation
        );
    }
};