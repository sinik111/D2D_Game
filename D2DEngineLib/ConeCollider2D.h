#pragma once

#include "Collider.h"
#include "Cone2D.h"

class ConeCollider2D :
	public Collider
{
private:
	Cone2D m_cone;

public:
	const Cone2D& GetCone() const;

	void SetCone(float radius, const Vector2& direction, float angle);

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