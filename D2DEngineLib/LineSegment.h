#pragma once

#include "Vector2.h"

struct LineSegment
{
	Vector2 startPoint;
	Vector2 endPoint;
	Vector2 normal;

	LineSegment()
	{

	};

	LineSegment(const Vector2& startPoint, const Vector2& endPoint)
		: startPoint{ startPoint }, endPoint{ endPoint }
	{
		CalculateNormal();
	}

	void SetPoints(const Vector2& startPoint, const Vector2& endPoint)
	{
		this->startPoint = startPoint;
		this->endPoint = endPoint;

		CalculateNormal();
	}

	Bounds GetBounds() const
	{
		float minX = std::min<float>(startPoint.x, endPoint.x);
		float minY = std::min<float>(startPoint.y, endPoint.y);
		float maxX = std::max<float>(startPoint.x, endPoint.x);
		float maxY = std::max<float>(startPoint.y, endPoint.y);

		return Bounds::FromMinMax({ minX, minY }, { maxX, maxY });
	}

	Bounds GetBounds(const Vector2& worldPosition) const
	{
		Vector2 worldStartPoint = startPoint + worldPosition;
		Vector2 worldEndPoint = endPoint + worldPosition;

		float minX = std::min<float>(worldStartPoint.x, worldEndPoint.x);
		float minY = std::min<float>(worldStartPoint.y, worldEndPoint.y);
		float maxX = std::max<float>(worldStartPoint.x, worldEndPoint.x);
		float maxY = std::max<float>(worldStartPoint.y, worldEndPoint.y);

		return Bounds::FromMinMax({ minX, minY }, { maxX, maxY });
	}

	float LengthSq() const
	{
		return (endPoint - startPoint).LengthSq();
	}

	float Length() const
	{
		return (endPoint - startPoint).Length();
	}

private:
	void CalculateNormal()
	{
		Vector2 direction = (endPoint - startPoint).Normalized();

		normal.x = direction.y;
		normal.y = -direction.x;
	}
};
