#pragma once

#include "Collider.h"
#include "Sector.h"

class SectorCollider :
	public Collider
{
private:
	Sector m_sector;

public:
	const Sector& GetSector() const;

	void SetSector(float radius, const Vector2& direction, float angle);

public:
	void UpdateCollider() override;
	void CalculateSpatialBounds() override;

	CollisionInfo DetectCollision(const Collider* other) override;
	CollisionInfo DetectCollision(const BoxCollider2D* other) override;
	CollisionInfo DetectCollision(const CircleCollider* other) override;
	CollisionInfo DetectCollision(const LineCollider* other) override;
	CollisionInfo DetectCollision(const SectorCollider* other) override;
};