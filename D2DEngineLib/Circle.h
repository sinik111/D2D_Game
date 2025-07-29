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

	// TODO : ���� ����ü���� �浹 ���� �����
	void IsCollidedWithLine(LineSegment& line)
	{
		//line�� �̷�� ����
		Vector2 lineVector = line.endPoint - line.startPoint;
		float dx = lineVector.x;
		float dy = lineVector.y;

		//������ ����
		float lensq = lineVector.LengthSq();

		//������ ������
		float t = ((center.x - line.startPoint.x) * dx + (center.y - line.startPoint.y) * dy) / lensq;

		//���ͷκ��� ���� ����� ������ x,y �� ���ϱ�
		float closestX = line.startPoint.x + t * dx;
		float closestY = line.startPoint.y + t * dy;

		// ���� �������κ��� ���� ����� x,y ������ �Ÿ�
		float distX = center.x - closestX;
		float distY = center.y - closestY;
		float distSq = distX * distX + distY * distY;
		line.normal = { distX, distY };

		//�о ��ġ ���ϱ�
		float restX = radius - distX;
		float restY = radius - distY;

		// TODO : �浹 ������ Physics�� �ִ� Collision Info�� �����ؾ� ��
		if (distSq <= radius * radius)
		{
			center.x += restX;
			center.y += restY;
		}
	}
};