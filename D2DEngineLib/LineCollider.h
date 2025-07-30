#pragma once

#include "Collider.h"
#include "LineSegment.h"

class LineCollider :
	public Collider
{
private:
	LineSegment m_lineSegment;

public:
	const LineSegment& GetLineSegment() const;

	void SetLine(const Vector2& startPoint, const Vector2& endPoint);

public:
	void Render() override;
	void UpdateCollider() override;
	void CalculateSpatialBounds() override;

	CollisionInfo DetectCollision(const Collider* other) const override;
	CollisionInfo DetectCollision(const BoxCollider2D* other) const override;
	CollisionInfo DetectCollision(const CircleCollider* other) const override;
	CollisionInfo DetectCollision(const LineCollider* other) const override;
	CollisionInfo DetectCollision(const ConeCollider2D* other) const override;
};