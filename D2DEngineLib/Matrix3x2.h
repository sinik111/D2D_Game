#pragma once

#include <d2d1.h>
#include <cassert>

#include "MyMath.h"
#include "Vector2.h"

struct Matrix3x2 :
    public D2D1_MATRIX_3X2_F
{
public:
    //union
    //{
    //    struct
    //    {
    //        float _11, _12;
    //        float _21, _22;
    //        float _31, _32;
    //    };

    //    struct
    //    {
    //        Vector2 r[3];
    //    };

    //    float m[3][2];
    //};

public:
	Matrix3x2()
        : D2D1_MATRIX_3X2_F{}
	{

	}

    Matrix3x2(float m11, float m12, float m21, float m22, float m31, float m32)
        : D2D1_MATRIX_3X2_F{ m11, m12, m21, m22, m31, m32 }
	{

	}

public: // 연산자 오버로딩
    Matrix3x2 operator*(const Matrix3x2& rhs) const
    {
       return Matrix3x2(
            _11 * rhs._11 + _12 * rhs._21, _11 * rhs._12 + _12 * rhs._22,
            _21 * rhs._11 + _22 * rhs._21, _21 * rhs._12 + _22 * rhs._22,
            _31 * rhs._11 + _32 * rhs._21 + rhs._31, _31 * rhs._12 + _32 * rhs._22 + rhs._32
        );
    }

    Matrix3x2& operator*=(const Matrix3x2& rhs)
    {
        return (*this = *this * rhs);
    }

    //operator D2D1_MATRIX_3X2_F() const
    //{
    //    return D2D1_MATRIX_3X2_F{ _11, _12, _21, _22, _31, _32 };
    //}

public: // 유틸리티 함수
    static Matrix3x2 Translation(float x, float y)
    {
        return Matrix3x2(1.0f, 0.0f, 0.0f, 1.0f, x, y);
    }

    static Matrix3x2 Translation(const Vector2& position)
    {
        return Matrix3x2(1.0f, 0.0f, 0.0f, 1.0f, position.x, position.y);
    }

    static Matrix3x2 Rotation(float degree)
    {
        float radian = degree * MyMath::DegToRad;
        float cosResult = std::cos(radian);
        float sinResult = std::sin(radian);

        return Matrix3x2(cosResult, -sinResult, sinResult, cosResult, 0.0f, 0.0f);
    }

    static Matrix3x2 Scale(float x, float y)
    {
        return Matrix3x2(x, 0.0f, 0.0f, y, 0.0f, 0.0f);
    }

    static Matrix3x2 Scale(const Vector2& scale)
    {
        return Matrix3x2(scale.x, 0.0f, 0.0f, scale.y, 0.0f, 0.0f);
    }

    Matrix3x2 Inverse() const
    {
        float determinant = _11 * _22 - _12 * _21;

        if (std::abs(determinant) < MyMath::EPSILON)
        {
            assert(false && "0으로 나누고 있습니다");
            return Identity();
        }

        Matrix3x2 result;

        result._11 = _22 / determinant;
        result._12 = -_12 / determinant;
        result._21 = -_21 / determinant;
        result._22 = _11 / determinant;
        result._31 = -(_31 * result._11 + _32 * result._21);
        result._32 = -(_31 * result._12 + _32 * result._22);

        return result;
    }

    static Matrix3x2 Identity()
    {
        return Matrix3x2(1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
    }

    Vector2 TransformPoint(const Vector2& point) const
    {
        return Vector2(point.x * _11 + point.y * _21 + _31, point.x * _12 + point.y * _22 + _32);
    }

    Vector2 TransformVector(const Vector2& v) const
    {
        return Vector2(v.x * _11 + v.y * _21, v.x * _12 + v.y * _22);
    }

    Vector2 GetScale() const
    {
        return Vector2(std::sqrt(_11 * _11 + _12 * _12), std::sqrt(_21 * _21 + _22 * _22));
    }

    Vector2 GetPosition() const
    {
        return Vector2(_31, _32);
    }

    float GetRotation() const
    {
        Vector2 xAxis{ _11, _12 };

        float currentScaleX = xAxis.Length();
        if (std::abs(currentScaleX) < MyMath::EPSILON)
        {
            return 0.0f;
        }
        xAxis *= (1.0f / currentScaleX);

        float radian = std::atan2(xAxis.y, xAxis.x);

        return radian * MyMath::RadToDeg;
    }

    void ResetScale(float scaleX = 1.0f, float scaleY = 1.0f)
    {
        Vector2 xAxis{ _11, _12 };
        Vector2 yAxis{ _21, _22 };

        float currentScaleX = xAxis.Length();
        float currentScaleY = yAxis.Length();

        float invScaleX = (currentScaleX != 0.0f) ? (1.0f / currentScaleX) : 0.0f;
        float invScaleY = (currentScaleY != 0.0f) ? (1.0f / currentScaleY) : 0.0f;

        xAxis *= invScaleX;
        yAxis *= invScaleY;

        xAxis *= scaleX;
        yAxis *= scaleY;

        _11 = xAxis.x;
        _12 = xAxis.y;
        _21 = yAxis.x;
        _22 = yAxis.y;
    }
};