#include "pch.h"
#include "Physics.h"

#include "BoxCollider2D.h"
#include "RigidBody2D.h"
#include "GameObject.h"

const Vector2 Physics::gravity{ 0.0f, -500.0f };

bool Physics::DetectCollision(const BoxCollider2D* a, const BoxCollider2D* b, CollisionInfo& info)
{
	const Bounds& boundsA = a->GetBounds();
	const Bounds& boundsB = b->GetBounds();

	Vector2 delta = Vector2::Abs(boundsA.center - boundsB.center);
	Vector2 total = boundsA.extents + boundsB.extents;

	if (delta.x <= total.x && delta.y <= total.y)
	{
		info.isCollide = true;

		float xAxisDepth = total.x - delta.x;
		float yAxisDepth = total.y - delta.y;

		if (xAxisDepth < yAxisDepth)
		{
			if (boundsA.center.x < boundsB.center.x)
			{
				info.normal = Vector2::Right;
			}
			else
			{
				info.normal = Vector2::Left;
			}

			info.penetrationDepth = xAxisDepth;
		}
		else
		{
			if (boundsA.center.y < boundsB.center.y)
			{
				info.normal = Vector2::Up;
			}
			else
			{
				info.normal = Vector2::Down;
			}

			info.penetrationDepth = yAxisDepth;
		}

		info.colliderA = a;
		info.colliderB = b;

		info.rigidBodyA = a->GetRigidBody2D();
		info.rigidBodyB = b->GetRigidBody2D();

		// 겹치는 영역의 X, Y 최소/최대값

		Vector2 aMin = boundsA.GetMin();
		Vector2 aMax = boundsA.GetMax();
		Vector2 bMin = boundsB.GetMin();
		Vector2 bMax = boundsB.GetMax();


		float overlapMinX = std::max<float>(aMin.x, bMin.x);
		float overlapMaxX = std::min<float>(aMax.x, bMax.x);
		float overlapMinY = std::max<float>(aMin.y, bMin.y);
		float overlapMaxY = std::min<float>(aMax.y, bMax.y);

		// 겹치는 영역의 중심을 접점으로 사용
		info.contactPoint = Vector2((overlapMinX + overlapMaxX) * 0.5f, (overlapMinY + overlapMaxY) * 0.5f);

		//info.contactPoint = boundsA.center - info.normal * info.penetrationDepth * 0.5f;

		return true;
	}
	
	return false;
}

void Physics::ResolveCollision(const CollisionInfo& info)
{
	if (!info.isCollide)
		return;

	RigidBody2D* rigidBodyA = info.rigidBodyA;
	RigidBody2D* rigidBodyB = info.rigidBodyB;

	float inverseMassA = (rigidBodyA && rigidBodyA->GetBodyType() == RigidBody2D::BodyType::Dynamic) ? (1.0f / rigidBodyA->GetMass()) : 0.0f;
	float inverseMassB = (rigidBodyB && rigidBodyB->GetBodyType() == RigidBody2D::BodyType::Dynamic) ? (1.0f / rigidBodyB->GetMass()) : 0.0f;

	float totalInverseMass = inverseMassA + inverseMassB;

	if (totalInverseMass < MyMath::EPSILON)
	{
		if (rigidBodyA && rigidBodyA->GetBodyType() == RigidBody2D::BodyType::Static && rigidBodyB && rigidBodyB->GetBodyType() == RigidBody2D::BodyType::Dynamic)
		{
			rigidBodyB->SetPosition(rigidBodyB->GetPosition() + info.normal * info.penetrationDepth);
		}
		else if (rigidBodyB && rigidBodyB->GetBodyType() == RigidBody2D::BodyType::Static && rigidBodyA && rigidBodyA->GetBodyType() == RigidBody2D::BodyType::Dynamic)
		{
			rigidBodyA->SetPosition(rigidBodyA->GetPosition() - info.normal * info.penetrationDepth);
		}
		
		return;
	}

	float moveAmountA = info.penetrationDepth * (inverseMassA / totalInverseMass);
	float moveAmountB = info.penetrationDepth * (inverseMassB / totalInverseMass);

	if (rigidBodyA && rigidBodyA->GetBodyType() == RigidBody2D::BodyType::Dynamic)
	{
		rigidBodyA->SetPosition(rigidBodyA->GetPosition() - info.normal * moveAmountA);
		Vector2 velocitiy = rigidBodyA->GetVelocity();

		if (info.normal.y < 0.0f && velocitiy.y < 0.0f || info.normal.y > 0.0f && velocitiy.y > 0.0f)
		{
			velocitiy.y = 0.0f;
		}

		if (info.normal.x < 0.0f && velocitiy.x < 0.0f || info.normal.x > 0.0f && velocitiy.x > 0.0f)
		{
			velocitiy.x = 0.0f;
		}

		rigidBodyA->SetVelocity(velocitiy);
	}

	if (rigidBodyB && rigidBodyB->GetBodyType() == RigidBody2D::BodyType::Dynamic)
	{
		rigidBodyB->SetPosition(rigidBodyB->GetPosition() + info.normal * moveAmountB);
		Vector2 velocitiy = rigidBodyB->GetVelocity();

		if (-info.normal.y < 0.0f && velocitiy.y < 0.0f || -info.normal.y > 0.0f && velocitiy.y > 0.0f)
		{
			velocitiy.y = 0.0f;
		}

		if (-info.normal.x < 0.0f && velocitiy.x < 0.0f || -info.normal.x > 0.0f && velocitiy.x > 0.0f)
		{
			velocitiy.x = 0.0f;
		}
		
		rigidBodyB->SetVelocity(velocitiy);
	}
}
