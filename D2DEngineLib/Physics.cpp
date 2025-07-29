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
std::map<CollisionLayer, unsigned int> Physics::s_collisionMasks;

void Physics::SetupCollisionMatrix()
{
	s_collisionMasks[CollisionLayer::PlayerHitBox] =
		static_cast<unsigned int>(CollisionLayer::EnemyAttack);

	s_collisionMasks[CollisionLayer::EnemyHitBox] =
		static_cast<unsigned int>(CollisionLayer::PlayerAttack);

	s_collisionMasks[CollisionLayer::PlayerAttack] =
		static_cast<unsigned int>(CollisionLayer::EnemyHitBox);

	s_collisionMasks[CollisionLayer::EnemyAttack] =
		static_cast<unsigned int>(CollisionLayer::PlayerHitBox);

	s_collisionMasks[CollisionLayer::Building] =
		static_cast<unsigned int>(CollisionLayer::PlayerMove | CollisionLayer::EnemyMove);

	s_collisionMasks[CollisionLayer::Wall] =
		static_cast<unsigned int>(CollisionLayer::PlayerMove | CollisionLayer::EnemyMove);

	s_collisionMasks[CollisionLayer::PlayerMove] =
		static_cast<unsigned int>(CollisionLayer::Building | CollisionLayer::Wall | CollisionLayer::EnemyMove);

	s_collisionMasks[CollisionLayer::EnemyMove] =
		static_cast<unsigned int>(CollisionLayer::Building | CollisionLayer::Wall | CollisionLayer::PlayerMove);

	s_collisionMasks[CollisionLayer::Sensor] =
		static_cast<unsigned int>(CollisionLayer::PlayerMove);
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
		if (distance == 0.0f)
		{
			// �� ���� �߽��� ������ ��ĥ �� (��ħ�� �ſ� ����)
			// �� ��� ������ ��� �������ε� ���� ����.
			// ���� ���, (0, 1) �Ǵ� (1, 0)�� ���� ������ �������� �����ϰų�,
			// ���� �������� �ӵ� ���� ���� ����� �� ����.
			// ���⼭�� �����ϰ� Y�� ���� �������� ����
			info.normal = { 0.0f, 1.0f };
		}
		else
		{
			info.normal = displacement.Normalized(); // circleA���� circleB�� ���ϴ� ���� ����
		}

		// 7. �浹 ����
		// CircleA�� ��迡�� ���� �������� CircleA�� ��������ŭ ������ ���� (��, CircleA�� "�浹 ��" ����)
		info.contactPoint = circleA.center + info.normal * circleA.radius;
		// �Ǵ� �� ���� �߽��� �մ� ���� ������ �浹�� �Ͼ�� ���� (���ġ)
		// info.contactPoint = circleA.center + info.normal * (circleA.radius - info.penetrationDepth / 2.0f);

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

	// ��� ������ �и� ���� �����մϴ�.
	// �� �ﰢ���� ���, �� �𼭸��� ������ ���� �˴ϴ�.
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
		axis = axis.Normalized(); // ���� ���ͷ� ����ȭ

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

	// ������ �Ϸ�Ǹ�, �и� ���� ã�� ���ߴٴ� �ǹ��Դϴ�.
	// ���� �浹�� �߻��߽��ϴ�.
	info.isCollide = true;
	info.penetrationDepth = minOverlap;
	info.normal = collisionNormal.Normalized(); // ���� ������ ���� ���ͷ� ����ȭ

	// ����(Contact Point) ���:
	// SAT�� �Ϲ������� MTV(Minimum Translation Vector)�� �����ϸ�, �������� ������ �������� �ʽ��ϴ�.
	// �ﰢ���� ��� ��Ȯ�� ����(���� ����, �𼭸� �߰��� �Ǵ� ���� ���� ���� �Ŵ����带 ����)��
	// �����ϴ� ���� �� �����ϸ�, �Ϲ������� GJK-EPA ���� �Ǵ� Ŭ���ΰ� ���� �߰� �˰����� �ʿ��մϴ�.

	// ���⼭�� �����ϰ� ������ ������ ����մϴ�.
	// coneA�� �߽ɿ��� ���� �ݴ� �������� ���������� ħ�� ���̸� �� ��ŭ �̵��� �����Դϴ�.
	// �̴� A�� �浹 ��輱 ������ �з����� �� ������ ��Ÿ���ϴ�.
	info.contactPoint = verticesA[0] + info.normal * (coneA.radius - info.penetrationDepth);
	// �� contactPoint�� �ٻ�ġ�̸�, �浹�� ������ ������� �𼭸�-�𼭸� �Ǵ� ����-�������� �߻��� ���
	// ���������� ��Ȯ�� ������ �ƴ� �� �ֽ��ϴ�.

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
		if (distance == 0.0f)
		{
			// ���� �߽��� AABB ���ο� ���� �� (��Ȯ�� AABB�� ��� �� ���� ���� ����� ��)
			// �� ��� ������ �ణ�� heuristics�� �ʿ�.
			// ������ AABB�� � �鿡 ���� ������� �Ǵ��Ͽ� �� ���� ������ ���.
			// ���⼭�� �����ϰ� AABB�� �� ��(min.x, min.y)���� �߽ɱ����� ���͸� ����ȭ�Ͽ� ����ϰų�,
			// ���� ���� ��ġ�� ���� ã�� �ش� ���� ������ ���.
			// ���� ���, AABB�� �߽ɿ��� ���� �߽� �������� ������ ������ �� ����.

			Vector2 aabbCenter = { (boxMin.x + boxMax.x) / 2.0f, (boxMin.y + boxMax.y) / 2.0f };
			info.normal = (circle.center - aabbCenter).Normalized();
			if (info.normal.LengthSq() == 0.0f) // ���� ���� �߽��� AABB �߽ɰ� ��ġ��
			{
				info.normal = { 0.0f, 1.0f }; // �⺻������ ���� ���� ����
			}
		}
		else
		{
			info.normal = displacement.Normalized(); // �� �߽ɿ��� closestPoint�� ���ϴ� ������ ����ȭ
		}

		// 6. �浹 ���� (Contact Point)
		// ���� �Ϲ����� ����: ���� ��� �� AABB�� ���� ����� ����
		info.contactPoint = circle.center - info.normal * circle.radius;
		// �ٸ� ����: closestPoint (AABB���� ���� ���� ����� ����)
		// info.contactPoint = closestPoint;

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
		if (distance == 0.0f)
		{
			// ���� �߽��� AABB ���ο� ���� �� (��Ȯ�� AABB�� ��� �� ���� ���� ����� ��)
			// �� ��� ������ �ణ�� heuristics�� �ʿ�.
			// ������ AABB�� � �鿡 ���� ������� �Ǵ��Ͽ� �� ���� ������ ���.
			// ���⼭�� �����ϰ� AABB�� �� ��(min.x, min.y)���� �߽ɱ����� ���͸� ����ȭ�Ͽ� ����ϰų�,
			// ���� ���� ��ġ�� ���� ã�� �ش� ���� ������ ���.
			// ���� ���, AABB�� �߽ɿ��� ���� �߽� �������� ������ ������ �� ����.

			Vector2 aabbCenter = { (boxMin.x + boxMax.x) / 2.0f, (boxMin.y + boxMax.y) / 2.0f };
			info.normal = (circle.center - aabbCenter).Normalized();
			if (info.normal.LengthSq() == 0.0f) // ���� ���� �߽��� AABB �߽ɰ� ��ġ��
			{
				info.normal = { 0.0f, 1.0f }; // �⺻������ ���� ���� ����
			}
		}
		else
		{
			info.normal = displacement.Normalized(); // �� �߽ɿ��� closestPoint�� ���ϴ� ������ ����ȭ
		}

		// 6. �浹 ���� (Contact Point)
		// ���� �Ϲ����� ����: ���� ��� �� AABB�� ���� ����� ����
		info.contactPoint = circle.center - info.normal * circle.radius;
		// �ٸ� ����: closestPoint (AABB���� ���� ���� ����� ����)
		// info.contactPoint = closestPoint;

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
		// ���� ������ '����' (normal�� ����Ű�� ����)���� ħ���ߴ��� Ȯ��
		// ���� �߽��� ���� normal �������� �󸶳� ������ �ִ��� (����� normal ����, ������ �ݴ� ����)
		float projDistance = Vector2::Dot(distVec, segment.normal);

		// ���� ���� �߽��� ���� normal�� �ݴ� �������� ħ���߰ų�,
		// (projDistance + circle.radius)�� ����� �ƴϸ� �浹�� �������� ���� �� ����.
		// ������ ����� ��û�� ���� "������ ������ ���������� �а�" �Ϸ���,
		// �Ÿ������� �浹 ���θ� �Ǵ��ϰ� ���� ������ �����ϴ� ���� �½��ϴ�.
		// ���⼭�� ���� ���п� ��⸸ �ص� �浹�� �����մϴ�.

		info.isCollide = true;

		// �浹 ������ ������ LineSegment�� normal �������� ����
		info.normal = segment.normal;

		// ħ�� ���� ���:
		// ���� �߽��� ���� normal �������� �󸶳� ���� �İ����°�?
		// ���� �߽ɰ� ���� ������ '�Ÿ�'�� distVec.Length() �Դϴ�.
		// �� �Ÿ��� ���� '����'���� �󸶳� ���Դ��� ����մϴ�.

		// ���� �߽ɿ��� ���� normal ���������� ���ʹ� distVec �� �ƴմϴ�.
		// distVec�� closestPoint���� �����Դϴ�.
		// ���� �߽ɿ��� ���� (���)������ �ִ� �Ÿ��� ���ؾ� �մϴ�.
		// �̸� ���� (���� �߽� - ���� ���� ������ ��)�� ���� normal�� dot product�� ����մϴ�.
		float signedDistanceFromLine = Vector2::Dot((circle.center - segment.startPoint), segment.normal);

		// ���� LineSegment�� normal �������� �İ����� ���� ��ȿ�� ���̸� �����ϴ�.
		// ��������ŭ ħ���ؾ� �浹�� ���۵˴ϴ�.
		// ���� ���, signedDistanceFromLine�� 0���� ������ ���� �߽��� �� �ٱ��ʿ� �ֽ��ϴ�.
		// ���� �������� signedDistanceFromLine�� ���밪�� ���������� �۾ƾ� �մϴ�.

		// penetrationDepth�� '�� ����' ���������� ħ�� ����
		// signedDistanceFromLine�� ��� (normal ���� ����)�� ���, ħ�� ���̴� (������ - signedDistanceFromLine)
		// signedDistanceFromLine�� ���� (normal ���� �ٱ���)�� ���, ħ�� ���̴� (������ + abs(signedDistanceFromLine))
		// �Ǵ� simply: (������ - signedDistanceFromLine)

		// ���� �߽��� ���� normal �������� (�� ��������) �󸶳� ���� ���Դ��� ���
		// ���� ���� ��� �����ϴ� ���� signedDistanceFromLine == radius
		// ���� ���� ������ ����ϸ� signedDistanceFromLine == -radius (�� �ʸ�)
		info.penetrationDepth = circle.radius - signedDistanceFromLine;

		// ���� (contactPoint):
		// ���� �߽ɿ��� �浹 ����(segment.normal)�� �ݴ� �������� ��������ŭ �̵��� ����
		// �� ������ ���� ���� ��� "�̷�����" ������ �˴ϴ�.
		info.contactPoint = circle.center - segment.normal * circle.radius;

		info.colliderA = a;
		info.colliderB = b;

		info.rigidBodyA = a->GetRigidBody2D();
		info.rigidBodyB = b->GetRigidBody2D();
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
		// ���� ������ '����' (normal�� ����Ű�� ����)���� ħ���ߴ��� Ȯ��
		// ���� �߽��� ���� normal �������� �󸶳� ������ �ִ��� (����� normal ����, ������ �ݴ� ����)
		float projDistance = Vector2::Dot(distVec, segment.normal);

		// ���� ���� �߽��� ���� normal�� �ݴ� �������� ħ���߰ų�,
		// (projDistance + circle.radius)�� ����� �ƴϸ� �浹�� �������� ���� �� ����.
		// ������ ����� ��û�� ���� "������ ������ ���������� �а�" �Ϸ���,
		// �Ÿ������� �浹 ���θ� �Ǵ��ϰ� ���� ������ �����ϴ� ���� �½��ϴ�.
		// ���⼭�� ���� ���п� ��⸸ �ص� �浹�� �����մϴ�.

		info.isCollide = true;

		// �浹 ������ ������ LineSegment�� normal �������� ����
		info.normal = segment.normal;

		// ħ�� ���� ���:
		// ���� �߽��� ���� normal �������� �󸶳� ���� �İ����°�?
		// ���� �߽ɰ� ���� ������ '�Ÿ�'�� distVec.Length() �Դϴ�.
		// �� �Ÿ��� ���� '����'���� �󸶳� ���Դ��� ����մϴ�.

		// ���� �߽ɿ��� ���� normal ���������� ���ʹ� distVec �� �ƴմϴ�.
		// distVec�� closestPoint���� �����Դϴ�.
		// ���� �߽ɿ��� ���� (���)������ �ִ� �Ÿ��� ���ؾ� �մϴ�.
		// �̸� ���� (���� �߽� - ���� ���� ������ ��)�� ���� normal�� dot product�� ����մϴ�.
		float signedDistanceFromLine = Vector2::Dot((circle.center - segment.startPoint), segment.normal);

		// ���� LineSegment�� normal �������� �İ����� ���� ��ȿ�� ���̸� �����ϴ�.
		// ��������ŭ ħ���ؾ� �浹�� ���۵˴ϴ�.
		// ���� ���, signedDistanceFromLine�� 0���� ������ ���� �߽��� �� �ٱ��ʿ� �ֽ��ϴ�.
		// ���� �������� signedDistanceFromLine�� ���밪�� ���������� �۾ƾ� �մϴ�.

		// penetrationDepth�� '�� ����' ���������� ħ�� ����
		// signedDistanceFromLine�� ��� (normal ���� ����)�� ���, ħ�� ���̴� (������ - signedDistanceFromLine)
		// signedDistanceFromLine�� ���� (normal ���� �ٱ���)�� ���, ħ�� ���̴� (������ + abs(signedDistanceFromLine))
		// �Ǵ� simply: (������ - signedDistanceFromLine)

		// ���� �߽��� ���� normal �������� (�� ��������) �󸶳� ���� ���Դ��� ���
		// ���� ���� ��� �����ϴ� ���� signedDistanceFromLine == radius
		// ���� ���� ������ ����ϸ� signedDistanceFromLine == -radius (�� �ʸ�)
		info.penetrationDepth = circle.radius - signedDistanceFromLine;

		// ���� (contactPoint):
		// ���� �߽ɿ��� �浹 ����(segment.normal)�� �ݴ� �������� ��������ŭ �̵��� ����
		// �� ������ ���� ���� ��� "�̷�����" ������ �˴ϴ�.
		info.contactPoint = circle.center - segment.normal * circle.radius;

		info.colliderA = a;
		info.colliderB = b;

		info.rigidBodyA = a->GetRigidBody2D();
		info.rigidBodyB = b->GetRigidBody2D();
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

	// Cone�� �𼭸� �� �߰� (����ȭ�� �� ���͸� ���)
	axes.push_back((coneVertices[1] - coneVertices[0]).Normalized()); // cone edge 0-1
	axes.push_back((coneVertices[2] - coneVertices[1]).Normalized()); // cone edge 1-2
	axes.push_back((coneVertices[0] - coneVertices[2]).Normalized()); // cone edge 2-0

	// �� ������ �и� ���� ��ȸ
	for (const auto& axis : axes) // AABB�� ���� �̹� ����ȭ�Ǿ� ����. Cone�� ���� Normalize() ȣ�� �ʿ�.
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

		// ��ģ�ٸ� ���� ��ħ ���� ���
		const float currentOverlap = projBox.GetOverlap(projCone);

		// �ּ� ��ħ ������Ʈ
		if (currentOverlap < minOverlap)
		{
			minOverlap = currentOverlap;
			collisionNormal = axis;

			// ���� ���� ����: box�� �߽ɿ��� cone�� �߽� ��������
			// AABB�� �߽��� min�� max�� �߰���
			Vector2 centerBox{ (min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f };
			Vector2 centerCone = cone.center;
			Vector2 dirBoxToCone = centerCone - centerBox;

			if (Vector2::Dot(collisionNormal, dirBoxToCone) < 0)
			{
				collisionNormal = collisionNormal * -1.0f; // ���� ���� ������
			}
		}
	}

	// ��� �࿡�� ���ƴٸ� �浹 �߻�
	info.isCollide = true;
	info.penetrationDepth = minOverlap;
	info.normal = collisionNormal.Normalized(); // ���� ������ ���� ����

	// ����(Contact Point) ���:
	// AABB�� �ﰢ���� �浹 ���� ����� �����ϸ�, SAT�����δ� ������� �ʽ��ϴ�.
	// ���⼭�� ������ ����ġ�� ����մϴ�.
	// AABB�� �߽ɿ��� ���� �ݴ� �������� ħ�� ���̸� ����Ͽ� �̵��� ����
	Vector2 centerBox = { (min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f };
	info.contactPoint = centerBox + info.normal * (minOverlap);
	// �̴� �ٻ�ġ�̸�, ��Ȯ�� ������ �� ������ �˰���(GJK-EPA, Ŭ����)�� �ʿ��մϴ�.

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

	// AABB�� ���� �׻� X��� Y���� �����Դϴ�. (0,1)�� (1,0) �� ����
	axes.push_back({ 1.0f, 0.0f }); // X��
	axes.push_back({ 0.0f, 1.0f }); // Y��

	// Cone�� �𼭸� �� �߰� (����ȭ�� �� ���͸� ���)
	axes.push_back((coneVertices[1] - coneVertices[0]).Normalized()); // cone edge 0-1
	axes.push_back((coneVertices[2] - coneVertices[1]).Normalized()); // cone edge 1-2
	axes.push_back((coneVertices[0] - coneVertices[2]).Normalized()); // cone edge 2-0

	// �� ������ �и� ���� ��ȸ
	for (const auto& axis : axes) // AABB�� ���� �̹� ����ȭ�Ǿ� ����. Cone�� ���� Normalize() ȣ�� �ʿ�.
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

		// ��ģ�ٸ� ���� ��ħ ���� ���
		const float currentOverlap = projBox.GetOverlap(projCone);

		// �ּ� ��ħ ������Ʈ
		if (currentOverlap < minOverlap)
		{
			minOverlap = currentOverlap;
			collisionNormal = axis;

			// ���� ���� ����: box�� �߽ɿ��� cone�� �߽� ��������
			// AABB�� �߽��� min�� max�� �߰���
			Vector2 centerBox{ (min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f };
			Vector2 centerCone = cone.center;
			Vector2 dirBoxToCone = centerCone - centerBox;

			if (Vector2::Dot(collisionNormal, dirBoxToCone) < 0)
			{
				collisionNormal = collisionNormal * -1.0f; // ���� ���� ������
			}
		}
	}

	// ��� �࿡�� ���ƴٸ� �浹 �߻�
	info.isCollide = true;
	info.penetrationDepth = minOverlap;
	info.normal = collisionNormal.Normalized(); // ���� ������ ���� ����

	// ����(Contact Point) ���:
	// AABB�� �ﰢ���� �浹 ���� ����� �����ϸ�, SAT�����δ� ������� �ʽ��ϴ�.
	// ���⼭�� ������ ����ġ�� ����մϴ�.
	// AABB�� �߽ɿ��� ���� �ݴ� �������� ħ�� ���̸� ����Ͽ� �̵��� ����
	Vector2 centerBox = { (min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f };
	info.contactPoint = centerBox + info.normal * (minOverlap);
	// �̴� �ٻ�ġ�̸�, ��Ȯ�� ������ �� ������ �˰���(GJK-EPA, Ŭ����)�� �ʿ��մϴ�.

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

	if (lengthSq == 0.0f) // ������ ���� ���ų� ���̰� 0�� ���
	{
		t_out = 0.0f;

		return segment.startPoint;
	}

	// ���� ���� AB�� ������ ��ġ (0.0f: p1, 1.0f: p2)
	// t = (point - segment.p1).Dot(AB) / lengthSq;
	float t = Vector2::Dot((point - segment.startPoint), AB) / lengthSq;

	// t ���� [0, 1] ������ Ŭ�����Ͽ� ���� ���� ���� ����� ���� ã���� ����
	t_out = std::max<float>(0.0f, std::min<float>(1.0f, t));

	// ���� ���� ���� ����� ��
	return segment.startPoint + AB * t_out;
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

	//// rigidBodyA�� �ӵ� ó��
	//if (rigidBodyA && rigidBodyA->GetBodyType() == RigidBody2D::BodyType::Dynamic)
	//{
	//	Vector2 currentVelocityA = rigidBodyA->GetVelocity();
	//	// ���� �ӵ����� ���� ���������� ������ ���
	//	Vector2 velocityNormalComponentA = info.normal * Vector2::Dot(currentVelocityA, info.normal);
	//	// ���� ���� �ӵ� ���и� ���� (���� ���� �ӵ� ����)
	//	Vector2 velocityTangentComponentA = currentVelocityA - velocityNormalComponentA;
	//	rigidBodyA->SetVelocity(velocityTangentComponentA);
	//}

	//// rigidBodyB�� �ӵ� ó��
	//if (rigidBodyB && rigidBodyB->GetBodyType() == RigidBody2D::BodyType::Dynamic)
	//{
	//	Vector2 currentVelocityB = rigidBodyB->GetVelocity();
	//	// ���� �ӵ����� ���� ���������� ������ ���
	//	// B�� A�κ��� info.normal �������� �з����� ���̹Ƿ�, B�� ���������� -info.normal �������� �浹��
	//	// ������ �ӵ��� ������ ���� ������ normal ���͸� ����ص� �˴ϴ�.
	//	// �߿��� ���� B�� info.normal �������� '�з�����' ���� �����ϴ� ���Դϴ�.
	//	Vector2 velocityNormalComponentB = info.normal * Vector2::Dot(currentVelocityB, info.normal);
	//	// ���� ���� �ӵ� ���и� ���� (���� ���� �ӵ� ����)
	//	Vector2 velocityTangentComponentB = currentVelocityB - velocityNormalComponentB;
	//	rigidBodyB->SetVelocity(velocityTangentComponentB);
	//}

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
