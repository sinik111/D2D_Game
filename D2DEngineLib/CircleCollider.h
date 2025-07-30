#pragma once

#include "Collider.h"
#include "Circle.h"

class CircleCollider :
	public Collider
{
private:
	Circle m_circle;

public:
	const Circle& GetCircle() const;

	void SetRadius(float radius);

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

