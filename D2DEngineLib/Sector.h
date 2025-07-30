#pragma once

#include "MyMath.h"
#include "Vector2.h"

struct Sector
{
	Vector2 center{};
	float radius = 0.0f;
	Vector2 direction{};
	float angle = 0.0f;

	Sector()
	{

	}

	Sector(const Vector2& center, float radius, const Vector2& direction, float angle)
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

	Bounds GetBounds() const
	{
		// 1. ���� �� �� ���� ��� (��� 0 ~ 2PI�� ����ȭ)
		float mainAngleRad = atan2(direction.y, direction.x);
		float startAngleRad = MyMath::NormalizeRadian(mainAngleRad - angle * MyMath::DegToRad / 2.0f);
		float endAngleRad = MyMath::NormalizeRadian(mainAngleRad + angle * MyMath::DegToRad / 2.0f);

		float minX = center.x;
		float minY = center.y;
		float maxX = center.x;
		float maxY = center.y;

		// 2. �� ���� ���� ���� ����
		Vector2 startPoint = { center.x + radius * cos(startAngleRad), center.y + radius * sin(startAngleRad) };
		Vector2 endPoint = { center.x + radius * cos(endAngleRad), center.y + radius * sin(endAngleRad) };

		minX = std::min<float>({ minX, startPoint.x, endPoint.x });
		minY = std::min<float>({ minY, startPoint.y, endPoint.y });
		maxX = std::max<float>({ maxX, startPoint.x, endPoint.x });
		maxY = std::max<float>({ maxY, startPoint.y, endPoint.y });

		// 3. ȣ(Arc) �κ��� ���� ����
		float cardinalAngles[] = { 0.0f, MyMath::PI * 0.5f, MyMath::PI * 1.0f, MyMath::PI * 1.5f }; // 0, 90, 180, 270��

		for (float angle : cardinalAngles)
		{
			if (IsAngleInSector(angle, startAngleRad, endAngleRad))
			{
				Vector2 pointOnArc = { center.x + radius * cos(angle), center.y + radius * sin(angle) };
				minX = std::min<float>(minX, pointOnArc.x);
				minY = std::min<float>(minY, pointOnArc.y);
				maxX = std::max<float>(maxX, pointOnArc.x);
				maxY = std::max<float>(maxY, pointOnArc.y);
			}
		}

		return Bounds::FromMinMax({ minX, minY }, { maxX, maxY });
	}
};