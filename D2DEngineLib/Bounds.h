#pragma once

#include <d2d1.h>

#include "Vector2.h"
#include "Matrix3x2.h"

struct Bounds
{
	Vector2 center;
	Vector2 extents;

	Bounds()
		: center{}, extents{}
	{

	}

	Bounds(const Vector2& center, const Vector2& extents)
		: center{ center }, extents{ extents }
	{

	}

	Vector2 GetSize() const
	{
		return extents * 2.0f;
	}

	Vector2 GetMin() const
	{
		return center - extents;
	}

	Vector2 GetMax() const
	{
		return center + extents;
	}

	static Bounds RectToWorldBounds(const D2D1_RECT_F& localRect, const Matrix3x2& worldMatrix)
	{
		Vector2 corners[]{
			Vector2(localRect.left, localRect.top),
			Vector2(localRect.right, localRect.top),
			Vector2(localRect.right, localRect.bottom),
			Vector2(localRect.left, localRect.bottom)
		};

		float minX = INFINITY;
		float minY = INFINITY;
		float maxX = -INFINITY;
		float maxY = -INFINITY;

		for (auto& corner : corners)
		{
			corner = worldMatrix.TransformPoint(corner);

			if (corner.x < minX)
			{
				minX = corner.x;
			}

			if (corner.x > maxX)
			{
				maxX = corner.x;
			}

			if (corner.y < minY)
			{
				minY = corner.y;
			}

			if (corner.y > maxY)
			{
				maxY = corner.y;
			}
		}

		return FromMinMax(Vector2(minX, minY), Vector2(maxX, maxY));
	}

	static Bounds FromMinMax(const Vector2& min, const Vector2& max)
	{
		Vector2 c = (min + max) * 0.5f;
		Vector2 e = (max - min) * 0.5f;

		return Bounds(c, e);
	}

	static bool IsOverlap(const Bounds& a, const Bounds& b)
	{
		Vector2 delta = Vector2::Abs(a.center - b.center);
		Vector2 total = a.extents + b.extents;

		return delta.x <= total.x && delta.y <= total.y;
	}

	bool IsContain(const Vector2& point) const
	{
		Vector2 delta = Vector2::Abs(center - point);

		return delta.x <= extents.x && delta.y <= extents.y;
	}

	bool IsContain(const Bounds& other) const
	{
		Vector2 myMin = GetMin();
		Vector2 myMax = GetMax();

		Vector2 otherMin = other.GetMin();
		Vector2 otherMax = other.GetMax();

		return myMin.x <= otherMin.x && myMax.x >= otherMax.x &&
			myMin.y <= otherMin.y && myMax.y >= otherMax.y;
	}

	void GetVertices(Vector2 outVertices[4]) const
	{
		Vector2 min = GetMin();
		Vector2 max = GetMax();

		outVertices[0] = { min.x, min.y }; // Bottom-left
		outVertices[1] = { max.x, min.y }; // Bottom-right
		outVertices[2] = { max.x, max.y }; // Top-right
		outVertices[3] = { min.x, max.y }; // Top-left
	}
};