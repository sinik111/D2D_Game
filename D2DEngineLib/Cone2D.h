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
		// ��� ������ 0 ~ 2PI�� ����ȭ
		angleRad = MyMath::NormalizeRadian(angleRad);
		sectorStartAngleRad = MyMath::NormalizeRadian(sectorStartAngleRad);
		sectorEndAngleRad = MyMath::NormalizeRadian(sectorEndAngleRad);

		if (sectorStartAngleRad <= sectorEndAngleRad)
		{
			// �Ϲ����� ���: [start, end]
			return (angleRad >= sectorStartAngleRad && angleRad <= sectorEndAngleRad);
		}
		else
		{
			// 0���� ���������� ���: [start, 2PI) U [0, end]
			return (angleRad >= sectorStartAngleRad || angleRad <= sectorEndAngleRad);
		}
	}

	bool ContainsPoint(const Vector2& point) const {
		// 1. �Ÿ� �˻�
		if ((point - center).LengthSq() > radius * radius + MyMath::EPSILON)
		{
			return false;
		}

		// 2. ���� �˻�
		Vector2 toPoint = point - center;
		if (toPoint.LengthSq() < MyMath::EPSILON * MyMath::EPSILON)  // ���� �߽ɿ� �ʹ� ����� �� (0���� ���� ����)
		{
			return true; // �߽��� �׻� ��ä�ÿ� ���Եȴٰ� ����
		}
		float pointAngle = atan2(toPoint.y, toPoint.x);

		return IsAngleInSector(pointAngle, GetStartRadian(), GetEndRadian());
	}

	Bounds GetBounds() const
	{
		Vector2 vertices[3];
		GetVertices(vertices); // �ﰢ���� �� ������ �����ɴϴ�.

		// ù ��° �������� min/max�� �ʱ�ȭ�մϴ�.
		float minX = vertices[0].x;
		float minY = vertices[0].y;
		float maxX = vertices[0].x;
		float maxY = vertices[0].y;

		// ������ �������� ��ȸ�ϸ� ��ü �ּ�/�ִ� ���� ã���ϴ�.
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