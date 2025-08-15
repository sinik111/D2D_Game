#include "pch.h"
#include "Physics.h"

#include "BoxCollider2D.h"
#include "RigidBody2D.h"
#include "GameObject.h"
#include "CircleCollider.h"
#include "ConeCollider2D.h"
#include "LineCollider.h"

#include "Circle.h"

const Vector2 Physics::gravity{ 0.0f, -500.0f };
std::unordered_map<CollisionLayer, unsigned int> Physics::s_collisionMasks;

void Physics::SetupCollisionMatrix()
{
	s_collisionMasks[CollisionLayer::PlayerHitBox] =
		static_cast<unsigned int>(CollisionLayer::EnemyAttack);

	s_collisionMasks[CollisionLayer::EnemyHitBox] =
		static_cast<unsigned int>(CollisionLayer::PlayerAttack);

	s_collisionMasks[CollisionLayer::PlayerAttack] =
		static_cast<unsigned int>(CollisionLayer::EnemyHitBox | CollisionLayer::EnemyAttack);

	s_collisionMasks[CollisionLayer::EnemyAttack] =
		static_cast<unsigned int>(CollisionLayer::PlayerHitBox | CollisionLayer::PlayerAttack);

	s_collisionMasks[CollisionLayer::Building] =
		static_cast<unsigned int>(CollisionLayer::PlayerMove | CollisionLayer::EnemyMove);

	s_collisionMasks[CollisionLayer::Wall] =
		static_cast<unsigned int>(CollisionLayer::PlayerMove | CollisionLayer::EnemyMove);

	s_collisionMasks[CollisionLayer::PlayerMove] =
		static_cast<unsigned int>(CollisionLayer::Building | CollisionLayer::Wall | CollisionLayer::EnemyMove | CollisionLayer::Sensor);

	s_collisionMasks[CollisionLayer::EnemyMove] =
		static_cast<unsigned int>(CollisionLayer::Building | CollisionLayer::Wall | CollisionLayer::PlayerMove);

	s_collisionMasks[CollisionLayer::Sensor] =
		static_cast<unsigned int>(CollisionLayer::PlayerMove);
}

unsigned int Physics::GetCollisionMask(CollisionLayer layer)
{
	return s_collisionMasks[layer];
}

bool Physics::DetectCollision(const BoxCollider2D* a, const BoxCollider2D* b, CollisionInfo& info)
{
	const Bounds& boundsA = a->GetBox();
	const Bounds& boundsB = b->GetBox();

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

		// ��ġ�� ������ X, Y �ּ�/�ִ밪

		Vector2 aMin = boundsA.GetMin();
		Vector2 aMax = boundsA.GetMax();
		Vector2 bMin = boundsB.GetMin();
		Vector2 bMax = boundsB.GetMax();


		float overlapMinX = std::max<float>(aMin.x, bMin.x);
		float overlapMaxX = std::min<float>(aMax.x, bMax.x);
		float overlapMinY = std::max<float>(aMin.y, bMin.y);
		float overlapMaxY = std::min<float>(aMax.y, bMax.y);

		// ��ġ�� ������ �߽��� �������� ���
		info.contactPoint = Vector2((overlapMinX + overlapMaxX) * 0.5f, (overlapMinY + overlapMaxY) * 0.5f);

		return true;
	}
	
	return false;
}

CollisionInfo Physics::DetectCollisionBoxBox(const BoxCollider2D* a, const BoxCollider2D* b)
{
	const Bounds& boundsA = a->GetBox();
	const Bounds& boundsB = b->GetBox();

	const Vector2 delta = Vector2::Abs(boundsA.center - boundsB.center);
	const Vector2 total = boundsA.extents + boundsB.extents;

	CollisionInfo info;

	if (delta.x <= total.x && delta.y <= total.y)
	{
		info.isCollide = true;

		const float xAxisDepth = total.x - delta.x;
		const float yAxisDepth = total.y - delta.y;

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

		// ��ġ�� ������ X, Y �ּ�/�ִ밪

		const Vector2 aMin = boundsA.GetMin();
		const Vector2 aMax = boundsA.GetMax();
		const Vector2 bMin = boundsB.GetMin();
		const Vector2 bMax = boundsB.GetMax();


		const float overlapMinX = std::max<float>(aMin.x, bMin.x);
		const float overlapMaxX = std::min<float>(aMax.x, bMax.x);
		const float overlapMinY = std::max<float>(aMin.y, bMin.y);
		const float overlapMaxY = std::min<float>(aMax.y, bMax.y);

		// ��ġ�� ������ �߽��� �������� ���
		info.contactPoint = Vector2((overlapMinX + overlapMaxX) * 0.5f, (overlapMinY + overlapMaxY) * 0.5f);

		info.colliderA = a;
		info.colliderB = b;

		info.rigidBodyA = a->GetRigidBody2D();
		info.rigidBodyB = b->GetRigidBody2D();

		return info;
	}

	info.isCollide = false;

	return info;
}

CollisionInfo Physics::DetectCollisionCircleCircle(const CircleCollider* a, const CircleCollider* b)
{
	const Circle& circleA = a->GetCircle();
	const Circle& circleB = b->GetCircle();

	// 1. �� ���� �߽� �� ���� ���
	Vector2 displacement = circleB.center - circleA.center;

	// 2. �Ÿ� ���� ��� (����)
	float distanceSquared = displacement.LengthSq();

	// 3. �������� �� ���
	float radiiSum = circleA.radius + circleB.radius;
	float radiiSumSquared = radiiSum * radiiSum;

	CollisionInfo info;

	// 4. �浹 ���� �Ǵ�
	if (distanceSquared <= radiiSumSquared)
	{
		info.isCollide = true;

		float distance = sqrt(distanceSquared);

		// 5. �İ�� �Ÿ�
		info.penetrationDepth = radiiSum - distance;

		// 6. �浹 ����
		if (distance < MyMath::EPSILON)
		{
			info.normal = { 0.0f, 1.0f };
		}
		else
		{
			info.normal = displacement.Normalized(); // circleA���� circleB�� ���ϴ� ���� ����
		}

		info.contactPoint = circleA.center + info.normal * circleA.radius;

		info.colliderA = a;
		info.colliderB = b;

		info.rigidBodyA = a->GetRigidBody2D();
		info.rigidBodyB = b->GetRigidBody2D();
	}

	return info;
}

CollisionInfo Physics::DetectCollisionConeCone(const ConeCollider2D* a, const ConeCollider2D* b)
{
	CollisionInfo info;
	info.isCollide = true; // �и� ���� ã�� �������� �浹�Ѵٰ� ����

	const Cone2D& coneA = a->GetCone();
	const Cone2D& coneB = b->GetCone();

	Vector2 verticesA[3];
	coneA.GetVertices(verticesA); // coneA�� �ﰢ�� ���� ��������
	Vector2 verticesB[3];
	coneB.GetVertices(verticesB); // coneB�� �ﰢ�� ���� ��������

	// �ּ� ħ�� ���� �� �ش� ������ ������ ����
	float minOverlap = FLT_MAX;
	Vector2 collisionNormal = { 0.0f, 0.0f };
	std::vector<Vector2> axes;

	// Cone A�� �𼭸� �� �߰�
	axes.push_back((verticesA[1] - verticesA[0])); // �𼭸� A0-A1 ����
	axes.push_back((verticesA[2] - verticesA[1])); // �𼭸� A1-A2 ����
	axes.push_back((verticesA[0] - verticesA[2])); // �𼭸� A2-A0 ����

	// Cone B�� �𼭸� �� �߰�
	axes.push_back((verticesB[1] - verticesB[0])); // �𼭸� B0-B1 ����
	axes.push_back((verticesB[2] - verticesB[1])); // �𼭸� B1-B2 ����
	axes.push_back((verticesB[0] - verticesB[2])); // �𼭸� B2-B0 ����

	// �� ������ �и� ���� ��ȸ�մϴ�.
	for (const auto& edgeVec : axes)
	{
		// �𼭸��� ������ ����(����)�� ����մϴ�.
		// ���� ������ ����ϱ� ���� ����ȭ�մϴ�.
		Vector2 axis{ -edgeVec.y, edgeVec.x }; // �ð� �ݴ� �������� 90�� ȸ��

		if (axis.LengthSq() < MyMath::EPSILON)
		{
			continue;
		}

		axis = axis.Normalized(); // ���� ���ͷ� ����ȭ	//zeroDivide �߻��� �� ����

		// �� �ﰢ���� ���� �࿡ �����մϴ�.
		Projection projA = ProjectPolygon(verticesA, 3, axis);
		Projection projB = ProjectPolygon(verticesB, 3, axis);

		// ������ ������ ��ġ�� ������ �и� ���� �����ϴ� ���Դϴ�.
		// �̴� �浹�� ������ �ǹ��մϴ�.
		if (!projA.Overlaps(projB))
		{
			info.isCollide = false;
			return info; // �浹 ����, ���� ����
		}

		// ��ģ�ٸ� ���� ��ħ ���� ����մϴ�.
		float currentOverlap = projA.GetOverlap(projB);

		// �� ��ħ�� ���ݱ��� ã�� �ּҰ����� �۴ٸ� ������Ʈ�մϴ�.
		if (currentOverlap < minOverlap)
		{
			minOverlap = currentOverlap;
			collisionNormal = axis;

			// ���� ���� ����: A�� �߽ɿ��� B�� �߽� ������ �����Ѿ� �մϴ�.
			// �̴� A�� B�κ��� �ùٸ��� �з����� ������ �����մϴ�.
			Vector2 centerA_tri = verticesA[0]; // Cone A�� ������
			Vector2 centerB_tri = verticesB[0]; // Cone B�� ������
			Vector2 dirAToB = centerB_tri - centerA_tri;

			// ������ A���� B�� ���ϴ� ����� �ݴ��̸� �������ϴ�.
			if (Vector2::Dot(collisionNormal, dirAToB) < 0)
			{
				collisionNormal = collisionNormal * -1.0f; // ���� ���� ������
			}
		}
	}

	info.isCollide = true;
	info.penetrationDepth = minOverlap;
	info.contactPoint = verticesA[0] + info.normal * (coneA.radius - info.penetrationDepth);

	info.colliderA = a;
	info.colliderB = b;

	info.rigidBodyA = a->GetRigidBody2D();
	info.rigidBodyB = b->GetRigidBody2D();

	return info;
}

CollisionInfo Physics::DetectCollisionBoxCircle(const BoxCollider2D* a, const CircleCollider* b)
{
	const Bounds& box = a->GetBox();
	const Circle& circle = b->GetCircle();

	const Vector2 boxMin = box.GetMin();
	const Vector2 boxMax = box.GetMax();

	// 1. AABB ������ ���� �߽ɰ� ���� ����� �� ã��
	const float closestX = MyMath::Clamp(circle.center.x, boxMin.x, boxMax.x);
	const float closestY = MyMath::Clamp(circle.center.y, boxMin.y, boxMax.y);

	Vector2 closestPoint{ closestX, closestY };

	// 2. ���� ����� ���� ���� �߽� ���� �Ÿ� ���
	const Vector2 displacement = circle.center - closestPoint;
	const float distanceSquared = displacement.LengthSq();
	const float distance = std::sqrt(distanceSquared);

	const float radiusSquared = circle.radius * circle.radius;

	CollisionInfo info;

	// 3. �浹 ���� �Ǵ�
	if (distanceSquared <= radiusSquared)
	{
		info.isCollide = true;

		// 4. �İ�� �Ÿ� (Penetration Depth)
		info.penetrationDepth = circle.radius - distance;

		// 5. �浹 ���� (Normal)
		if (distance < MyMath::EPSILON)
		{
			Vector2 aabbCenter = { (boxMin.x + boxMax.x) / 2.0f, (boxMin.y + boxMax.y) / 2.0f };
			Vector2 direction = circle.center - aabbCenter;
			
			if (direction.LengthSq() < MyMath::EPSILON) // ���� ���� �߽��� AABB �߽ɰ� ��ġ��
			{
				info.normal = { 0.0f, 1.0f }; // �⺻������ ���� ���� ����
			}
			else
			{
				info.normal = (circle.center - aabbCenter).Normalized();
			}
		}
		else
		{
			if (distanceSquared < MyMath::EPSILON)
			{
				info.normal = { 0.0f, 1.0f }; // �⺻������ ���� ���� ����
			}
			else
			{
				info.normal = displacement.Normalized(); // �� �߽ɿ��� closestPoint�� ���ϴ� ������ ����ȭ
			}
		}



		info.contactPoint = circle.center - info.normal * circle.radius;

		info.colliderA = a;
		info.colliderB = b;

		info.rigidBodyA = a->GetRigidBody2D();
		info.rigidBodyB = b->GetRigidBody2D();
	}

	return info;
}

CollisionInfo Physics::DetectCollisionCircleBox(const CircleCollider* a, const BoxCollider2D* b)
{
	const Circle& circle = a->GetCircle();
	const Bounds& box = b->GetBox();

	const Vector2 boxMin = box.GetMin();
	const Vector2 boxMax = box.GetMax();

	// 1. AABB ������ ���� �߽ɰ� ���� ����� �� ã��
	const float closestX = MyMath::Clamp(circle.center.x, boxMin.x, boxMax.x);
	const float closestY = MyMath::Clamp(circle.center.y, boxMin.y, boxMax.y);

	Vector2 closestPoint{ closestX, closestY };

	// 2. ���� ����� ���� ���� �߽� ���� �Ÿ� ���
	const Vector2 displacement = circle.center - closestPoint;
	const float distanceSquared = displacement.LengthSq();
	const float distance = std::sqrt(distanceSquared);

	const float radiusSquared = circle.radius * circle.radius;

	CollisionInfo info;

	// 3. �浹 ���� �Ǵ�
	if (distanceSquared <= radiusSquared)
	{
		info.isCollide = true;

		// 4. �İ�� �Ÿ� (Penetration Depth)
		info.penetrationDepth = circle.radius - distance;

		// 5. �浹 ���� (Normal)
		if (distance < MyMath::EPSILON)
		{
			Vector2 aabbCenter = { (boxMin.x + boxMax.x) / 2.0f, (boxMin.y + boxMax.y) / 2.0f };
			Vector2 direction = circle.center - aabbCenter;

			if (direction.LengthSq() < MyMath::EPSILON) // ���� ���� �߽��� AABB �߽ɰ� ��ġ��
			{
				info.normal = { 0.0f, 1.0f }; // �⺻������ ���� ���� ����
			}
			else
			{
				info.normal = -(circle.center - aabbCenter).Normalized();
			}
		}
		else
		{
			if (distanceSquared < MyMath::EPSILON)
			{
				info.normal = { 0.0f, 1.0f }; // �⺻������ ���� ���� ����
			}
			else
			{
				info.normal = -displacement.Normalized(); // �� �߽ɿ��� closestPoint�� ���ϴ� ������ ����ȭ
			}
		}

		info.contactPoint = circle.center - info.normal * circle.radius;

		info.colliderA = a;
		info.colliderB = b;

		info.rigidBodyA = a->GetRigidBody2D();
		info.rigidBodyB = b->GetRigidBody2D();
	}

	return info;
}

CollisionInfo Physics::DetectCollisionLineCircle(const LineCollider* a, const CircleCollider* b)
{
	CollisionInfo info;

	const LineSegment& segment = a->GetLineSegment();
	const Circle& circle = b->GetCircle();

	float t; // ���� ������ ���� ����� ���� ����
	Vector2 closestPoint = GetClosestPointOnLineSegment(circle.center, segment, t);

	// ���� �߽ɿ��� ���� ���� ���� ����� �������� ����
	Vector2 distVec = circle.center - closestPoint;
	float distanceSq = distVec.LengthSq();
	float radiusSq = circle.radius * circle.radius;

	// �Ÿ��� ���������� ������ �浹
	if (distanceSq < radiusSq)
	{
		info.isCollide = true;

		//info.normal = distanceSq <= MyMath::EPSILON ? segment.normal : distVec.Normalized();
		info.normal = segment.normal;

		//float signedDistanceFromLine = Vector2::Dot((circle.center - segment.startPoint), segment.normal);

		info.penetrationDepth = circle.radius - distVec.Length();

		//info.contactPoint = circle.center - segment.normal * circle.radius;

		info.colliderA = a;
		info.colliderB = b;

		info.rigidBodyA = a->GetRigidBody2D();
		info.rigidBodyB = b->GetRigidBody2D();
	}

	auto rb = b->GetRigidBody2D();
	if (rb)
	{
		Vector2 lastPosition = rb->GetLastFramePosition() + b->GetOffset();
		Vector2 estimatePosition = rb->GetPosition() + b->GetOffset();

		Vector2 sweepLine = estimatePosition - lastPosition;
		Vector2 segmentLine = segment.endPoint - segment.startPoint;
		Vector2 startToStart = segment.startPoint - lastPosition;

		float sweepXsegment = Vector2::Cross(sweepLine, segmentLine);

		if (!(std::abs(sweepXsegment) < MyMath::EPSILON))
		{
			float t = Vector2::Cross(startToStart, segmentLine) / sweepXsegment;
			float u = Vector2::Cross(startToStart, sweepLine) / sweepXsegment;

			if (t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f)
			{
				info.isCollide = true;

				info.normal = segment.normal;

				float signedDistanceFromLine = Vector2::Dot((estimatePosition - segment.startPoint), segment.normal);

				info.penetrationDepth = circle.radius - signedDistanceFromLine;

				info.contactPoint = circle.center - segment.normal * circle.radius;

				info.colliderA = a;
				info.colliderB = b;

				info.rigidBodyA = a->GetRigidBody2D();
				info.rigidBodyB = b->GetRigidBody2D();
			}
		}
	}

	return info;
}

CollisionInfo Physics::DetectCollisionCircleLine(const CircleCollider* a, const LineCollider* b)
{
	CollisionInfo info;

	const Circle& circle = a->GetCircle();
	const LineSegment& segment = b->GetLineSegment();

	float t; // ���� ������ ���� ����� ���� ����
	Vector2 closestPoint = GetClosestPointOnLineSegment(circle.center, segment, t);

	// ���� �߽ɿ��� ���� ���� ���� ����� �������� ����
	Vector2 distVec = circle.center - closestPoint;
	float distanceSq = distVec.LengthSq();
	float radiusSq = circle.radius * circle.radius;

	// �Ÿ��� ���������� ������ �浹
	if (distanceSq < radiusSq)
	{
		info.isCollide = true;

		//info.normal = distanceSq <= MyMath::EPSILON ? -segment.normal : -distVec.Normalized();
		info.normal = -segment.normal;

		//float signedDistanceFromLine = Vector2::Dot((circle.center - segment.startPoint), segment.normal);

		info.penetrationDepth = circle.radius - distVec.Length();

		//info.contactPoint = circle.center - segment.normal * circle.radius;

		info.colliderA = a;
		info.colliderB = b;

		info.rigidBodyA = a->GetRigidBody2D();
		info.rigidBodyB = b->GetRigidBody2D();
	}

	auto rb = a->GetRigidBody2D();
	if (rb)
	{
		Vector2 lastPosition = rb->GetLastFramePosition() + a->GetOffset();
		Vector2 estimatePosition = rb->GetPosition() + a->GetOffset();

		Vector2 sweepLine = estimatePosition - lastPosition;
		Vector2 segmentLine = segment.endPoint - segment.startPoint;
		Vector2 startToStart = segment.startPoint - lastPosition;

		float sweepXsegment = Vector2::Cross(sweepLine, segmentLine);

		if (!(std::abs(sweepXsegment) < MyMath::EPSILON))
		{
			float t = Vector2::Cross(startToStart, segmentLine) / sweepXsegment;
			float u = Vector2::Cross(startToStart, sweepLine) / sweepXsegment;

			if (t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f)
			{
				info.isCollide = true;

				info.normal = -segment.normal;

				float signedDistanceFromLine = Vector2::Dot((estimatePosition - segment.startPoint), segment.normal);

				info.penetrationDepth = circle.radius - signedDistanceFromLine;

				info.contactPoint = circle.center - segment.normal * circle.radius;

				info.colliderA = a;
				info.colliderB = b;

				info.rigidBodyA = a->GetRigidBody2D();
				info.rigidBodyB = b->GetRigidBody2D();
			}
		}
	}

	return info;
}

CollisionInfo Physics::DetectCollisionBoxCone(const BoxCollider2D* a, const ConeCollider2D* b)
{
	CollisionInfo info;
	info.isCollide = true; // �и� ���� ã�� �������� �浹�Ѵٰ� ����

	const Bounds& box = a->GetBox();
	const Cone2D& cone = b->GetCone();

	const Vector2 min = box.GetMin();
	const Vector2 max = box.GetMax();

	Vector2 boxVertices[4];
	box.GetVertices(boxVertices); // AABB�� �� ���� ��������
	Vector2 coneVertices[3];
	cone.GetVertices(coneVertices); // Cone�� �� ���� ��������

	float minOverlap = FLT_MAX;
	Vector2 collisionNormal{ 0.0f, 0.0f };

	std::vector<Vector2> axes;

	// AABB�� ���� �׻� X��� Y���� �����Դϴ�. (0,1)�� (1,0) �� ����
	axes.push_back({ 1.0f, 0.0f }); // X��
	axes.push_back({ 0.0f, 1.0f }); // Y��

	axes.push_back(Vector2(-(coneVertices[1].y - coneVertices[0].y), coneVertices[1].x - coneVertices[0].x).Normalized());
	axes.push_back(Vector2(-(coneVertices[2].y - coneVertices[1].y), coneVertices[2].x - coneVertices[1].x).Normalized());
	axes.push_back(Vector2(-(coneVertices[0].y - coneVertices[2].y), coneVertices[0].x - coneVertices[2].x).Normalized());

	for (const auto& axis : axes)
	{
		// ���� (AABB�� ��� GetVertices�� ���� Polygonó�� ����)
		const Projection projBox = ProjectPolygon(boxVertices, 4, axis); // �ڽ��� 4���� ����
		const Projection projCone = ProjectPolygon(coneVertices, 3, axis); // ���� 3���� ����

		// ������ ��ġ�� ������ �и� ���� ���� -> �浹 ����
		if (!projBox.Overlaps(projCone))
		{
			info.isCollide = false;
			return info;
		}

		const float currentOverlap = projBox.GetOverlap(projCone);

		// �ּ� ��ħ ������Ʈ
		if (currentOverlap < minOverlap)
		{
			minOverlap = currentOverlap;
			collisionNormal = axis;

			Vector2 centerBox{ (min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f };
			Vector2 centerCone = cone.center;
			Vector2 dirBoxToCone = centerCone - centerBox;

			if (Vector2::Dot(collisionNormal, dirBoxToCone) < 0)
			{
				collisionNormal = collisionNormal * -1.0f; // ���� ���� ������
			}
		}
	}

	info.isCollide = true;
	info.penetrationDepth = minOverlap;
	info.normal = collisionNormal.Normalized(); // ���� ������ ���� ����

	Vector2 centerBox = { (min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f };
	info.contactPoint = centerBox + info.normal * (minOverlap);

	info.colliderA = a;
	info.colliderB = b;

	info.rigidBodyA = a->GetRigidBody2D();
	info.rigidBodyB = b->GetRigidBody2D();

	return info;
}

CollisionInfo Physics::DetectCollisionConeBox(const ConeCollider2D* a, const BoxCollider2D* b)
{
	CollisionInfo info;
	info.isCollide = true; // �и� ���� ã�� �������� �浹�Ѵٰ� ����

	const Cone2D& cone = a->GetCone();
	const Bounds& box = b->GetBox();

	const Vector2 min = box.GetMin();
	const Vector2 max = box.GetMax();

	Vector2 boxVertices[4];
	box.GetVertices(boxVertices); // AABB�� �� ���� ��������
	Vector2 coneVertices[3];
	cone.GetVertices(coneVertices); // Cone�� �� ���� ��������

	float minOverlap = FLT_MAX;
	Vector2 collisionNormal{ 0.0f, 0.0f };

	std::vector<Vector2> axes;

	axes.push_back({ 1.0f, 0.0f }); // X��
	axes.push_back({ 0.0f, 1.0f }); // Y��

	axes.push_back(Vector2(-(coneVertices[1].y - coneVertices[0].y), coneVertices[1].x - coneVertices[0].x).Normalized());
	axes.push_back(Vector2(-(coneVertices[2].y - coneVertices[1].y), coneVertices[2].x - coneVertices[1].x).Normalized());
	axes.push_back(Vector2(-(coneVertices[0].y - coneVertices[2].y), coneVertices[0].x - coneVertices[2].x).Normalized());

	for (const auto& axis : axes)
	{
		const Projection projBox = ProjectPolygon(boxVertices, 4, axis); // �ڽ��� 4���� ����
		const Projection projCone = ProjectPolygon(coneVertices, 3, axis); // ���� 3���� ����

		if (!projBox.Overlaps(projCone))
		{
			info.isCollide = false;
			return info;
		}

		// ��ģ�ٸ� ���� ��ħ ���� ���
		const float currentOverlap = projBox.GetOverlap(projCone);

		// �ּ� ��ħ ������Ʈ
		if (currentOverlap < minOverlap)
		{
			minOverlap = currentOverlap;
			collisionNormal = axis;

			Vector2 centerBox{ (min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f };
			Vector2 centerCone = cone.center;
			Vector2 dirBoxToCone = centerCone - centerBox;

			if (Vector2::Dot(collisionNormal, dirBoxToCone) < 0)
			{
				collisionNormal = collisionNormal * -1.0f; // ���� ���� ������
			}
		}
	}

	info.isCollide = true;
	info.penetrationDepth = minOverlap;
	info.normal = -collisionNormal.Normalized();

	Vector2 centerBox = { (min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f };
	info.contactPoint = centerBox + info.normal * (minOverlap);

	info.colliderA = a;
	info.colliderB = b;

	info.rigidBodyA = a->GetRigidBody2D();
	info.rigidBodyB = b->GetRigidBody2D();

	return info;
}

Projection Physics::ProjectPolygon(const Vector2* vertices, int numVertices, const Vector2& axis) {
	float min = Vector2::Dot(axis, vertices[0]);
	float max = min;
	for (int i = 1; i < numVertices; ++i)
	{
		float p = Vector2::Dot(axis, vertices[i]);
		min = std::min<float>(min, p);
		max = std::max<float>(max, p);
	}

	return { min, max };
}

Vector2 Physics::GetClosestPointOnLineSegment(const Vector2& point, const LineSegment& segment, float& t_out)
{
	Vector2 AB = segment.endPoint - segment.startPoint; // ���� ����
	float lengthSq = AB.LengthSq(); // ���� ������ ����

	if (lengthSq < MyMath::EPSILON) // ������ ���� ���ų� ���̰� 0�� ���
	{
		t_out = 0.0f;

		return segment.startPoint;
	}

	float t = Vector2::Dot((point - segment.startPoint), AB) / lengthSq;

	t_out = std::max<float>(0.0f, std::min<float>(1.0f, t));

	return segment.startPoint + AB * t_out;
}

void Physics::ResolveCollision(const CollisionInfo& info)
{
	if (!info.isCollide)
	{
		return;
	}

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
