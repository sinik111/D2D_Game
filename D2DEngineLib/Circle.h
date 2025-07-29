#pragma once

#include "Vector2.h"
#include "LineSegment.h"

struct Circle
{
	Vector2 center{};
	float radius{};

	Circle()
	{

	}

	Circle(const Vector2& center, float radius)
		: center{ center }, radius{ radius }
	{

	}

	Bounds GetBounds() const
	{
		float minX = center.x - radius;
		float minY = center.y - radius;
		float maxX = center.x + radius;
		float maxY = center.y + radius;

		return Bounds::FromMinMax({ minX, minY }, { maxX, maxY });
	}

	// TODO : 직선 구조체와의 충돌 판정 만들기
	void IsCollidedWithLine(LineSegment& line)
	{
		//line을 이루는 벡터
		Vector2 lineVector = line.endPoint - line.startPoint;
		float dx = lineVector.x;
		float dy = lineVector.y;

		//벡터의 길이
		float lensq = lineVector.LengthSq();

		//벡터의 방정식
		float t = ((center.x - line.startPoint.x) * dx + (center.y - line.startPoint.y) * dy) / lensq;

		//벡터로부터 가장 가까운 직선의 x,y 값 구하기
		float closestX = line.startPoint.x + t * dx;
		float closestY = line.startPoint.y + t * dy;

		// 원의 원점으로부터 가장 가까운 x,y 까지의 거리
		float distX = center.x - closestX;
		float distY = center.y - closestY;
		float distSq = distX * distX + distY * distY;
		line.normal = { distX, distY };

		//밀어낼 수치 구하기
		float restX = radius - distX;
		float restY = radius - distY;

		// TODO : 충돌 정보를 Physics에 있는 Collision Info로 전달해야 함
		if (distSq <= radius * radius)
		{
			center.x += restX;
			center.y += restY;
		}
	}
};