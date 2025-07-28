#pragma once
#include <d2d1.h>

#include "Vector2.h"
#include "Line.h"
struct Circle
{
	Vector2 Position;
	float Radius;

	// TODO : 직선 구조체와의 충돌 판정 만들기
	void IsCollidedWithLine(LineVector& line)
	{
		//line을 이루는 벡터
		Vector2 lineVector = line.EndPoint - line.StartPoint;
		float dx = lineVector.x;
		float dy = lineVector.y;

		//벡터의 길이
		float lensq = lineVector.LengthSq();

		//벡터의 방정식
		float t = ((Position.x - line.StartPoint.x) * dx + (Position.y - line.StartPoint.y) * dy) / lensq;

		//벡터로부터 가장 가까운 직선의 x,y 값 구하기
		float closestX = line.StartPoint.x + t * dx;
		float closestY = line.StartPoint.y + t * dy;

		// 원의 원점으로부터 가장 가까운 x,y 까지의 거리
		float distX = Position.x - closestX;
		float distY = Position.y - closestY;
		float distSq = distX * distX + distY * distY;
		line.NormalVector = { distX, distY };

		//밀어낼 수치 구하기
		float restX = Radius - distX;
		float restY = Radius - distY;

		// TODO : 충돌 정보를 Physics에 있는 Collision Info로 전달해야 함
		if (distSq <= Radius * Radius)
		{
			Position.x += restX;
			Position.y += restY;
		}
	}
};