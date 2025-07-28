#pragma once
#include <d2d1.h>

#include "Vector2.h"
#include "Line.h"
struct Circle
{
	Vector2 Position;
	float Radius;

	// TODO : ���� ����ü���� �浹 ���� �����
	void IsCollidedWithLine(LineVector& line)
	{
		//line�� �̷�� ����
		Vector2 lineVector = line.EndPoint - line.StartPoint;
		float dx = lineVector.x;
		float dy = lineVector.y;

		//������ ����
		float lensq = lineVector.LengthSq();

		//������ ������
		float t = ((Position.x - line.StartPoint.x) * dx + (Position.y - line.StartPoint.y) * dy) / lensq;

		//���ͷκ��� ���� ����� ������ x,y �� ���ϱ�
		float closestX = line.StartPoint.x + t * dx;
		float closestY = line.StartPoint.y + t * dy;

		// ���� �������κ��� ���� ����� x,y ������ �Ÿ�
		float distX = Position.x - closestX;
		float distY = Position.y - closestY;
		float distSq = distX * distX + distY * distY;
		line.NormalVector = { distX, distY };

		//�о ��ġ ���ϱ�
		float restX = Radius - distX;
		float restY = Radius - distY;

		// TODO : �浹 ������ Physics�� �ִ� Collision Info�� �����ؾ� ��
		if (distSq <= Radius * Radius)
		{
			Position.x += restX;
			Position.y += restY;
		}
	}
};