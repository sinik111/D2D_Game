#pragma once

#include "Collider.h"

class BoxCollider2D :
	public Collider
{
private:
	Bounds m_box;

public:
	const Bounds& GetBox() const;

	void SetSize(const Vector2& size);

public:
	void UpdateCollider() override;
	void CalculateSpatialBounds() override;

	CollisionInfo DetectCollision(const Collider* other) const override;
	CollisionInfo DetectCollision(const BoxCollider2D* other) const override;
	CollisionInfo DetectCollision(const CircleCollider* other) const override;
	CollisionInfo DetectCollision(const LineCollider* other) const override;
	CollisionInfo DetectCollision(const ConeCollider2D* other) const override;
};