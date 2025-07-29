#pragma once

#include "MyMath.h"
#include "Vector2.h"

struct Cone2D
{
	Vector2 center{};
	float radius = 0.0f;
	Vector2 direction{};
	float angle = 0.0f;

	Cone2D()
	{

	}

	Cone2D(const Vector2& center, float radius, const Vector2& direction, float angle)
		: center{ center }, radius{ radius }, direction{ direction }, angle{ angle }
	{

	}

	float GetStartRadian() const
	{
		float rawRadian = std::atan2(direction.y, direction.x) - angle * MyMath::DegToRad / 2.0f;

		return MyMath::NormalizeRadian(rawRadian);
	}

	float GetEndRadian() const
	{
		float rawRadian = std::atan2(direction.y, direction.x) + angle * MyMath::DegToRad / 2.0f;

		return MyMath::NormalizeRadian(rawRadian);
	}

	float GetStartDegree() const
	{
		float rawDegree = std::atan2(direction.y, direction.x) * MyMath::RadToDeg - angle * 2.0f;

		return MyMath::NormalizeRadian(rawDegree);
	}

	float GetEndDegree() const
	{
		float rawDegree = std::atan2(direction.y, direction.x) * MyMath::RadToDeg + angle * 2.0f;

		return MyMath::NormalizeRadian(rawDegree);
	}

	void GetVertices(Vector2 outVertices[3]) const
	{
		outVertices[0] = center;

		float startRadian = GetStartRadian();
		float endRadian = GetEndRadian();

		outVertices[1] = center + Vector2(radius * std::cos(startRadian), radius * std::sin(startRadian));
		outVertices[2] = center + Vector2(radius * std::cos(endRadian), radius * std::sin(endRadian));
	}

	static bool IsAngleInSector(float angleRad, float sectorStartAngleRad, float sectorEndAngleRad)
	{
		// 모든 각도를 0 ~ 2PI로 정규화
		angleRad = MyMath::NormalizeRadian(angleRad);
		sectorStartAngleRad = MyMath::NormalizeRadian(sectorStartAngleRad);
		sectorEndAngleRad = MyMath::NormalizeRadian(sectorEndAngleRad);

		if (sectorStartAngleRad <= sectorEndAngleRad)
		{
			// 일반적인 경우: [start, end]
			return (angleRad >= sectorStartAngleRad && angleRad <= sectorEndAngleRad);
		}
		else
		{
			// 0도를 가로지르는 경우: [start, 2PI) U [0, end]
			return (angleRad >= sectorStartAngleRad || angleRad <= sectorEndAngleRad);
		}
	}

	bool ContainsPoint(const Vector2& point) const {
		// 1. 거리 검사
		if ((point - center).LengthSq() > radius * radius + MyMath::EPSILON)
		{
			return false;
		}

		// 2. 각도 검사
		Vector2 toPoint = point - center;
		if (toPoint.LengthSq() < MyMath::EPSILON * MyMath::EPSILON)  // 점이 중심에 너무 가까울 때 (0길이 벡터 방지)
		{
			return true; // 중심은 항상 부채꼴에 포함된다고 가정
		}
		float pointAngle = atan2(toPoint.y, toPoint.x);

		return IsAngleInSector(pointAngle, GetStartRadian(), GetEndRadian());
	}

	Bounds GetBounds() const
	{
		Vector2 vertices[3];
		GetVertices(vertices); // 삼각형의 세 정점을 가져옵니다.

		// 첫 번째 정점으로 min/max를 초기화합니다.
		float minX = vertices[0].x;
		float minY = vertices[0].y;
		float maxX = vertices[0].x;
		float maxY = vertices[0].y;

		// 나머지 정점들을 순회하며 전체 최소/최대 값을 찾습니다.
		for (int i = 1; i < 3; ++i)
		{
			minX = std::min<float>(minX, vertices[i].x);
			minY = std::min<float>(minY, vertices[i].y);
			maxX = std::max<float>(maxX, vertices[i].x);
			maxY = std::max<float>(maxY, vertices[i].y);
		}

		return Bounds::FromMinMax({ minX, minY }, { maxX, maxY });
	}
};