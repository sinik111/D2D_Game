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

		// 겹치는 영역의 X, Y 최소/최대값

		const Vector2 aMin = boundsA.GetMin();
		const Vector2 aMax = boundsA.GetMax();
		const Vector2 bMin = boundsB.GetMin();
		const Vector2 bMax = boundsB.GetMax();


		const float overlapMinX = std::max<float>(aMin.x, bMin.x);
		const float overlapMaxX = std::min<float>(aMax.x, bMax.x);
		const float overlapMinY = std::max<float>(aMin.y, bMin.y);
		const float overlapMaxY = std::min<float>(aMax.y, bMax.y);

		// 겹치는 영역의 중심을 접점으로 사용
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

	// 1. 두 원의 중심 간 벡터 계산
	Vector2 displacement = circleB.center - circleA.center;

	// 2. 거리 제곱 계산 (성능)
	float distanceSquared = displacement.LengthSq();

	// 3. 반지름의 합 계산
	float radiiSum = circleA.radius + circleB.radius;
	float radiiSumSquared = radiiSum * radiiSum;

	CollisionInfo info;

	// 4. 충돌 여부 판단
	if (distanceSquared <= radiiSumSquared)
	{
		info.isCollide = true;

		float distance = sqrt(distanceSquared);

		// 5. 파고들어간 거리
		info.penetrationDepth = radiiSum - distance;

		// 6. 충돌 법선
		if (distance == 0.0f)
		{
			// 두 원의 중심이 완전히 겹칠 때 (겹침이 매우 심함)
			// 이 경우 법선은 어느 방향으로든 설정 가능.
			// 예를 들어, (0, 1) 또는 (1, 0)과 같은 임의의 방향으로 설정하거나,
			// 이전 프레임의 속도 방향 등을 고려할 수 있음.
			// 여기서는 간단하게 Y축 양의 방향으로 설정
			info.normal = { 0.0f, 1.0f };
		}
		else
		{
			info.normal = displacement.Normalized(); // circleA에서 circleB로 향하는 단위 벡터
		}

		// 7. 충돌 지점
		// CircleA의 경계에서 법선 방향으로 CircleA의 반지름만큼 떨어진 지점 (즉, CircleA의 "충돌 면" 지점)
		info.contactPoint = circleA.center + info.normal * circleA.radius;
		// 또는 두 원의 중심을 잇는 선분 위에서 충돌이 일어나는 지점 (평균치)
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
	info.isCollide = true; // 분리 축을 찾을 때까지는 충돌한다고 가정

	const Cone2D& coneA = a->GetCone();
	const Cone2D& coneB = b->GetCone();

	Vector2 verticesA[3];
	coneA.GetVertices(verticesA); // coneA의 삼각형 정점 가져오기
	Vector2 verticesB[3];
	coneB.GetVertices(verticesB); // coneB의 삼각형 정점 가져오기

	// 최소 침투 깊이 및 해당 법선을 저장할 변수
	float minOverlap = FLT_MAX;
	Vector2 collisionNormal = { 0.0f, 0.0f };

	// 모든 잠재적 분리 축을 수집합니다.
	// 두 삼각형의 경우, 각 모서리의 법선이 축이 됩니다.
	std::vector<Vector2> axes;

	// Cone A의 모서리 축 추가
	axes.push_back((verticesA[1] - verticesA[0])); // 모서리 A0-A1 벡터
	axes.push_back((verticesA[2] - verticesA[1])); // 모서리 A1-A2 벡터
	axes.push_back((verticesA[0] - verticesA[2])); // 모서리 A2-A0 벡터

	// Cone B의 모서리 축 추가
	axes.push_back((verticesB[1] - verticesB[0])); // 모서리 B0-B1 벡터
	axes.push_back((verticesB[2] - verticesB[1])); // 모서리 B1-B2 벡터
	axes.push_back((verticesB[0] - verticesB[2])); // 모서리 B2-B0 벡터

	// 각 잠재적 분리 축을 순회합니다.
	for (const auto& edgeVec : axes)
	{
		// 모서리에 수직인 벡터(법선)를 계산합니다.
		// 투영 축으로 사용하기 위해 정규화합니다.
		Vector2 axis{ -edgeVec.y, edgeVec.x }; // 시계 반대 방향으로 90도 회전
		axis = axis.Normalized(); // 단위 벡터로 정규화

		// 두 삼각형을 현재 축에 투영합니다.
		Projection projA = ProjectPolygon(verticesA, 3, axis);
		Projection projB = ProjectPolygon(verticesB, 3, axis);

		// 투영된 간격이 겹치지 않으면 분리 축이 존재하는 것입니다.
		// 이는 충돌이 없음을 의미합니다.
		if (!projA.Overlaps(projB))
		{
			info.isCollide = false;
			return info; // 충돌 없음, 조기 종료
		}

		// 겹친다면 현재 겹침 양을 계산합니다.
		float currentOverlap = projA.GetOverlap(projB);

		// 이 겹침이 지금까지 찾은 최소값보다 작다면 업데이트합니다.
		if (currentOverlap < minOverlap)
		{
			minOverlap = currentOverlap;
			collisionNormal = axis;

			// 법선 방향 조정: A의 중심에서 B의 중심 방향을 가리켜야 합니다.
			// 이는 A가 B로부터 올바르게 밀려나는 방향을 보장합니다.
			Vector2 centerA_tri = verticesA[0]; // Cone A의 꼭짓점
			Vector2 centerB_tri = verticesB[0]; // Cone B의 꼭짓점
			Vector2 dirAToB = centerB_tri - centerA_tri;

			// 법선이 A에서 B로 향하는 방향과 반대이면 뒤집습니다.
			if (Vector2::Dot(collisionNormal, dirAToB) < 0)
			{
				collisionNormal = collisionNormal * -1.0f; // 법선 방향 뒤집기
			}
		}
	}

	// 루프가 완료되면, 분리 축을 찾지 못했다는 의미입니다.
	// 따라서 충돌이 발생했습니다.
	info.isCollide = true;
	info.penetrationDepth = minOverlap;
	info.normal = collisionNormal.Normalized(); // 최종 법선을 단위 벡터로 정규화

	// 접점(Contact Point) 계산:
	// SAT는 일반적으로 MTV(Minimum Translation Vector)만 제공하며, 직접적인 접점을 제공하지 않습니다.
	// 삼각형의 경우 정확한 접점(단일 정점, 모서리 중간점 또는 여러 점이 접점 매니폴드를 형성)을
	// 결정하는 것은 더 복잡하며, 일반적으로 GJK-EPA 정제 또는 클리핑과 같은 추가 알고리즘이 필요합니다.

	// 여기서는 간단하게 추정된 접점을 계산합니다.
	// coneA의 중심에서 법선 반대 방향으로 반지름에서 침투 깊이를 뺀 만큼 이동한 지점입니다.
	// 이는 A가 충돌 경계선 밖으로 밀려나야 할 지점을 나타냅니다.
	info.contactPoint = verticesA[0] + info.normal * (coneA.radius - info.penetrationDepth);
	// 이 contactPoint는 근사치이며, 충돌이 복잡한 방식으로 모서리-모서리 또는 정점-정점으로 발생할 경우
	// 물리적으로 정확한 접점이 아닐 수 있습니다.

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

	// 1. AABB 내에서 원의 중심과 가장 가까운 점 찾기
	const float closestX = MyMath::Clamp(circle.center.x, boxMin.x, boxMax.x);
	const float closestY = MyMath::Clamp(circle.center.y, boxMin.y, boxMax.y);

	Vector2 closestPoint{ closestX, closestY };

	// 2. 가장 가까운 점과 원의 중심 간의 거리 계산
	const Vector2 displacement = circle.center - closestPoint;
	const float distanceSquared = displacement.LengthSq();
	const float distance = std::sqrt(distanceSquared);

	const float radiusSquared = circle.radius * circle.radius;

	CollisionInfo info;

	// 3. 충돌 여부 판단
	if (distanceSquared <= radiusSquared)
	{
		info.isCollide = true;

		// 4. 파고들어간 거리 (Penetration Depth)
		info.penetrationDepth = circle.radius - distance;

		// 5. 충돌 법선 (Normal)
		if (distance == 0.0f)
		{
			// 원의 중심이 AABB 내부에 있을 때 (정확히 AABB의 어느 한 점에 가장 가까울 때)
			// 이 경우 법선은 약간의 heuristics가 필요.
			// 보통은 AABB의 어떤 면에 가장 가까운지 판단하여 그 면의 법선을 사용.
			// 여기서는 간단하게 AABB의 한 점(min.x, min.y)에서 중심까지의 벡터를 정규화하여 사용하거나,
			// 가장 깊이 겹치는 축을 찾아 해당 축의 법선을 사용.
			// 예를 들어, AABB의 중심에서 원의 중심 방향으로 법선을 설정할 수 있음.

			Vector2 aabbCenter = { (boxMin.x + boxMax.x) / 2.0f, (boxMin.y + boxMax.y) / 2.0f };
			info.normal = (circle.center - aabbCenter).Normalized();
			if (info.normal.LengthSq() == 0.0f) // 만약 원의 중심이 AABB 중심과 겹치면
			{
				info.normal = { 0.0f, 1.0f }; // 기본값으로 위쪽 법선 설정
			}
		}
		else
		{
			info.normal = displacement.Normalized(); // 원 중심에서 closestPoint로 향하는 벡터의 정규화
		}

		// 6. 충돌 지점 (Contact Point)
		// 가장 일반적인 정의: 원의 경계 중 AABB와 가장 가까운 지점
		info.contactPoint = circle.center - info.normal * circle.radius;
		// 다른 정의: closestPoint (AABB에서 원에 가장 가까운 지점)
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

	// 1. AABB 내에서 원의 중심과 가장 가까운 점 찾기
	const float closestX = MyMath::Clamp(circle.center.x, boxMin.x, boxMax.x);
	const float closestY = MyMath::Clamp(circle.center.y, boxMin.y, boxMax.y);

	Vector2 closestPoint{ closestX, closestY };

	// 2. 가장 가까운 점과 원의 중심 간의 거리 계산
	const Vector2 displacement = circle.center - closestPoint;
	const float distanceSquared = displacement.LengthSq();
	const float distance = std::sqrt(distanceSquared);

	const float radiusSquared = circle.radius * circle.radius;

	CollisionInfo info;

	// 3. 충돌 여부 판단
	if (distanceSquared <= radiusSquared)
	{
		info.isCollide = true;

		// 4. 파고들어간 거리 (Penetration Depth)
		info.penetrationDepth = circle.radius - distance;

		// 5. 충돌 법선 (Normal)
		if (distance == 0.0f)
		{
			// 원의 중심이 AABB 내부에 있을 때 (정확히 AABB의 어느 한 점에 가장 가까울 때)
			// 이 경우 법선은 약간의 heuristics가 필요.
			// 보통은 AABB의 어떤 면에 가장 가까운지 판단하여 그 면의 법선을 사용.
			// 여기서는 간단하게 AABB의 한 점(min.x, min.y)에서 중심까지의 벡터를 정규화하여 사용하거나,
			// 가장 깊이 겹치는 축을 찾아 해당 축의 법선을 사용.
			// 예를 들어, AABB의 중심에서 원의 중심 방향으로 법선을 설정할 수 있음.

			Vector2 aabbCenter = { (boxMin.x + boxMax.x) / 2.0f, (boxMin.y + boxMax.y) / 2.0f };
			info.normal = (circle.center - aabbCenter).Normalized();
			if (info.normal.LengthSq() == 0.0f) // 만약 원의 중심이 AABB 중심과 겹치면
			{
				info.normal = { 0.0f, 1.0f }; // 기본값으로 위쪽 법선 설정
			}
		}
		else
		{
			info.normal = displacement.Normalized(); // 원 중심에서 closestPoint로 향하는 벡터의 정규화
		}

		// 6. 충돌 지점 (Contact Point)
		// 가장 일반적인 정의: 원의 경계 중 AABB와 가장 가까운 지점
		info.contactPoint = circle.center - info.normal * circle.radius;
		// 다른 정의: closestPoint (AABB에서 원에 가장 가까운 지점)
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

	float t; // 선분 위에서 가장 가까운 점의 비율
	Vector2 closestPoint = GetClosestPointOnLineSegment(circle.center, segment, t);

	// 원의 중심에서 선분 위의 가장 가까운 점으로의 벡터
	Vector2 distVec = circle.center - closestPoint;
	float distanceSq = distVec.LengthSq();
	float radiusSq = circle.radius * circle.radius;

	// 거리가 반지름보다 작으면 충돌
	if (distanceSq < radiusSq)
	{
		// 원이 선분의 '안쪽' (normal이 가리키는 방향)으로 침투했는지 확인
		// 원의 중심이 선분 normal 방향으로 얼마나 떨어져 있는지 (양수면 normal 방향, 음수면 반대 방향)
		float projDistance = Vector2::Dot(distVec, segment.normal);

		// 만약 원의 중심이 선분 normal과 반대 방향으로 침투했거나,
		// (projDistance + circle.radius)가 양수가 아니면 충돌로 간주하지 않을 수 있음.
		// 하지만 사용자 요청에 따라 "무조건 라인의 법선쪽으로 밀게" 하려면,
		// 거리만으로 충돌 여부를 판단하고 법선 방향은 고정하는 것이 맞습니다.
		// 여기서는 원이 선분에 닿기만 해도 충돌로 간주합니다.

		info.isCollide = true;

		// 충돌 법선은 무조건 LineSegment의 normal 방향으로 고정
		info.normal = segment.normal;

		// 침투 깊이 계산:
		// 원의 중심이 선분 normal 방향으로 얼마나 깊이 파고들었는가?
		// 원의 중심과 선분 사이의 '거리'는 distVec.Length() 입니다.
		// 이 거리를 통해 '안쪽'으로 얼마나 들어왔는지 계산합니다.

		// 원의 중심에서 선분 normal 방향으로의 벡터는 distVec 이 아닙니다.
		// distVec는 closestPoint로의 벡터입니다.
		// 원의 중심에서 선분 (평면)으로의 최단 거리를 구해야 합니다.
		// 이를 위해 (원의 중심 - 선분 위의 임의의 점)과 선분 normal의 dot product를 사용합니다.
		float signedDistanceFromLine = Vector2::Dot((circle.center - segment.startPoint), segment.normal);

		// 원이 LineSegment의 normal 방향으로 파고들었을 때만 유효한 깊이를 가집니다.
		// 반지름만큼 침투해야 충돌이 시작됩니다.
		// 예를 들어, signedDistanceFromLine이 0보다 작으면 원의 중심이 벽 바깥쪽에 있습니다.
		// 벽에 닿으려면 signedDistanceFromLine의 절대값이 반지름보다 작아야 합니다.

		// penetrationDepth는 '벽 안쪽' 방향으로의 침투 깊이
		// signedDistanceFromLine이 양수 (normal 방향 안쪽)일 경우, 침투 깊이는 (반지름 - signedDistanceFromLine)
		// signedDistanceFromLine이 음수 (normal 방향 바깥쪽)일 경우, 침투 깊이는 (반지름 + abs(signedDistanceFromLine))
		// 또는 simply: (반지름 - signedDistanceFromLine)

		// 원의 중심이 선분 normal 방향으로 (벽 안쪽으로) 얼마나 깊이 들어왔는지 계산
		// 원이 벽에 닿기 시작하는 순간 signedDistanceFromLine == radius
		// 원이 벽을 완전히 통과하면 signedDistanceFromLine == -radius (벽 너머)
		info.penetrationDepth = circle.radius - signedDistanceFromLine;

		// 접점 (contactPoint):
		// 원의 중심에서 충돌 법선(segment.normal)과 반대 방향으로 반지름만큼 이동한 지점
		// 이 지점이 원이 벽에 닿는 "이론적인" 지점이 됩니다.
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

	float t; // 선분 위에서 가장 가까운 점의 비율
	Vector2 closestPoint = GetClosestPointOnLineSegment(circle.center, segment, t);

	// 원의 중심에서 선분 위의 가장 가까운 점으로의 벡터
	Vector2 distVec = circle.center - closestPoint;
	float distanceSq = distVec.LengthSq();
	float radiusSq = circle.radius * circle.radius;

	// 거리가 반지름보다 작으면 충돌
	if (distanceSq < radiusSq)
	{
		// 원이 선분의 '안쪽' (normal이 가리키는 방향)으로 침투했는지 확인
		// 원의 중심이 선분 normal 방향으로 얼마나 떨어져 있는지 (양수면 normal 방향, 음수면 반대 방향)
		float projDistance = Vector2::Dot(distVec, segment.normal);

		// 만약 원의 중심이 선분 normal과 반대 방향으로 침투했거나,
		// (projDistance + circle.radius)가 양수가 아니면 충돌로 간주하지 않을 수 있음.
		// 하지만 사용자 요청에 따라 "무조건 라인의 법선쪽으로 밀게" 하려면,
		// 거리만으로 충돌 여부를 판단하고 법선 방향은 고정하는 것이 맞습니다.
		// 여기서는 원이 선분에 닿기만 해도 충돌로 간주합니다.

		info.isCollide = true;

		// 충돌 법선은 무조건 LineSegment의 normal 방향으로 고정
		info.normal = segment.normal;

		// 침투 깊이 계산:
		// 원의 중심이 선분 normal 방향으로 얼마나 깊이 파고들었는가?
		// 원의 중심과 선분 사이의 '거리'는 distVec.Length() 입니다.
		// 이 거리를 통해 '안쪽'으로 얼마나 들어왔는지 계산합니다.

		// 원의 중심에서 선분 normal 방향으로의 벡터는 distVec 이 아닙니다.
		// distVec는 closestPoint로의 벡터입니다.
		// 원의 중심에서 선분 (평면)으로의 최단 거리를 구해야 합니다.
		// 이를 위해 (원의 중심 - 선분 위의 임의의 점)과 선분 normal의 dot product를 사용합니다.
		float signedDistanceFromLine = Vector2::Dot((circle.center - segment.startPoint), segment.normal);

		// 원이 LineSegment의 normal 방향으로 파고들었을 때만 유효한 깊이를 가집니다.
		// 반지름만큼 침투해야 충돌이 시작됩니다.
		// 예를 들어, signedDistanceFromLine이 0보다 작으면 원의 중심이 벽 바깥쪽에 있습니다.
		// 벽에 닿으려면 signedDistanceFromLine의 절대값이 반지름보다 작아야 합니다.

		// penetrationDepth는 '벽 안쪽' 방향으로의 침투 깊이
		// signedDistanceFromLine이 양수 (normal 방향 안쪽)일 경우, 침투 깊이는 (반지름 - signedDistanceFromLine)
		// signedDistanceFromLine이 음수 (normal 방향 바깥쪽)일 경우, 침투 깊이는 (반지름 + abs(signedDistanceFromLine))
		// 또는 simply: (반지름 - signedDistanceFromLine)

		// 원의 중심이 선분 normal 방향으로 (벽 안쪽으로) 얼마나 깊이 들어왔는지 계산
		// 원이 벽에 닿기 시작하는 순간 signedDistanceFromLine == radius
		// 원이 벽을 완전히 통과하면 signedDistanceFromLine == -radius (벽 너머)
		info.penetrationDepth = circle.radius - signedDistanceFromLine;

		// 접점 (contactPoint):
		// 원의 중심에서 충돌 법선(segment.normal)과 반대 방향으로 반지름만큼 이동한 지점
		// 이 지점이 원이 벽에 닿는 "이론적인" 지점이 됩니다.
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
	info.isCollide = true; // 분리 축을 찾을 때까지는 충돌한다고 가정

	const Bounds& box = a->GetBox();
	const Cone2D& cone = b->GetCone();

	const Vector2 min = box.GetMin();
	const Vector2 max = box.GetMax();

	Vector2 boxVertices[4];
	box.GetVertices(boxVertices); // AABB의 네 정점 가져오기
	Vector2 coneVertices[3];
	cone.GetVertices(coneVertices); // Cone의 세 정점 가져오기

	float minOverlap = FLT_MAX;
	Vector2 collisionNormal{ 0.0f, 0.0f };

	std::vector<Vector2> axes;

	// AABB의 축은 항상 X축과 Y축의 법선입니다. (0,1)과 (1,0) 두 가지
	axes.push_back({ 1.0f, 0.0f }); // X축
	axes.push_back({ 0.0f, 1.0f }); // Y축

	// Cone의 모서리 축 추가 (정규화된 변 벡터를 사용)
	axes.push_back((coneVertices[1] - coneVertices[0]).Normalized()); // cone edge 0-1
	axes.push_back((coneVertices[2] - coneVertices[1]).Normalized()); // cone edge 1-2
	axes.push_back((coneVertices[0] - coneVertices[2]).Normalized()); // cone edge 2-0

	// 각 잠재적 분리 축을 순회
	for (const auto& axis : axes) // AABB의 축은 이미 정규화되어 있음. Cone의 축은 Normalize() 호출 필요.
	{
		// 투영 (AABB의 경우 GetVertices를 통해 Polygon처럼 투영)
		const Projection projBox = ProjectPolygon(boxVertices, 4, axis); // 박스는 4개의 정점
		const Projection projCone = ProjectPolygon(coneVertices, 3, axis); // 콘은 3개의 정점

		// 투영이 겹치지 않으면 분리 축이 존재 -> 충돌 없음
		if (!projBox.Overlaps(projCone))
		{
			info.isCollide = false;
			return info;
		}

		// 겹친다면 현재 겹침 양을 계산
		const float currentOverlap = projBox.GetOverlap(projCone);

		// 최소 겹침 업데이트
		if (currentOverlap < minOverlap)
		{
			minOverlap = currentOverlap;
			collisionNormal = axis;

			// 법선 방향 조정: box의 중심에서 cone의 중심 방향으로
			// AABB의 중심은 min과 max의 중간점
			Vector2 centerBox{ (min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f };
			Vector2 centerCone = cone.center;
			Vector2 dirBoxToCone = centerCone - centerBox;

			if (Vector2::Dot(collisionNormal, dirBoxToCone) < 0)
			{
				collisionNormal = collisionNormal * -1.0f; // 법선 방향 뒤집기
			}
		}
	}

	// 모든 축에서 겹쳤다면 충돌 발생
	info.isCollide = true;
	info.penetrationDepth = minOverlap;
	info.normal = collisionNormal.Normalized(); // 최종 법선은 단위 벡터

	// 접점(Contact Point) 계산:
	// AABB와 삼각형의 충돌 지점 계산은 복잡하며, SAT만으로는 충분하지 않습니다.
	// 여기서는 간단한 추정치를 사용합니다.
	// AABB의 중심에서 법선 반대 방향으로 침투 깊이를 고려하여 이동한 지점
	Vector2 centerBox = { (min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f };
	info.contactPoint = centerBox + info.normal * (minOverlap);
	// 이는 근사치이며, 정확한 접점은 더 복잡한 알고리즘(GJK-EPA, 클리핑)이 필요합니다.

	info.colliderA = a;
	info.colliderB = b;

	info.rigidBodyA = a->GetRigidBody2D();
	info.rigidBodyB = b->GetRigidBody2D();

	return info;
}

CollisionInfo Physics::DetectCollisionConeBox(const ConeCollider2D* a, const BoxCollider2D* b)
{
	CollisionInfo info;
	info.isCollide = true; // 분리 축을 찾을 때까지는 충돌한다고 가정

	const Cone2D& cone = a->GetCone();
	const Bounds& box = b->GetBox();

	const Vector2 min = box.GetMin();
	const Vector2 max = box.GetMax();

	Vector2 boxVertices[4];
	box.GetVertices(boxVertices); // AABB의 네 정점 가져오기
	Vector2 coneVertices[3];
	cone.GetVertices(coneVertices); // Cone의 세 정점 가져오기

	float minOverlap = FLT_MAX;
	Vector2 collisionNormal{ 0.0f, 0.0f };

	std::vector<Vector2> axes;

	// AABB의 축은 항상 X축과 Y축의 법선입니다. (0,1)과 (1,0) 두 가지
	axes.push_back({ 1.0f, 0.0f }); // X축
	axes.push_back({ 0.0f, 1.0f }); // Y축

	// Cone의 모서리 축 추가 (정규화된 변 벡터를 사용)
	axes.push_back((coneVertices[1] - coneVertices[0]).Normalized()); // cone edge 0-1
	axes.push_back((coneVertices[2] - coneVertices[1]).Normalized()); // cone edge 1-2
	axes.push_back((coneVertices[0] - coneVertices[2]).Normalized()); // cone edge 2-0

	// 각 잠재적 분리 축을 순회
	for (const auto& axis : axes) // AABB의 축은 이미 정규화되어 있음. Cone의 축은 Normalize() 호출 필요.
	{
		// 투영 (AABB의 경우 GetVertices를 통해 Polygon처럼 투영)
		const Projection projBox = ProjectPolygon(boxVertices, 4, axis); // 박스는 4개의 정점
		const Projection projCone = ProjectPolygon(coneVertices, 3, axis); // 콘은 3개의 정점

		// 투영이 겹치지 않으면 분리 축이 존재 -> 충돌 없음
		if (!projBox.Overlaps(projCone))
		{
			info.isCollide = false;
			return info;
		}

		// 겹친다면 현재 겹침 양을 계산
		const float currentOverlap = projBox.GetOverlap(projCone);

		// 최소 겹침 업데이트
		if (currentOverlap < minOverlap)
		{
			minOverlap = currentOverlap;
			collisionNormal = axis;

			// 법선 방향 조정: box의 중심에서 cone의 중심 방향으로
			// AABB의 중심은 min과 max의 중간점
			Vector2 centerBox{ (min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f };
			Vector2 centerCone = cone.center;
			Vector2 dirBoxToCone = centerCone - centerBox;

			if (Vector2::Dot(collisionNormal, dirBoxToCone) < 0)
			{
				collisionNormal = collisionNormal * -1.0f; // 법선 방향 뒤집기
			}
		}
	}

	// 모든 축에서 겹쳤다면 충돌 발생
	info.isCollide = true;
	info.penetrationDepth = minOverlap;
	info.normal = collisionNormal.Normalized(); // 최종 법선은 단위 벡터

	// 접점(Contact Point) 계산:
	// AABB와 삼각형의 충돌 지점 계산은 복잡하며, SAT만으로는 충분하지 않습니다.
	// 여기서는 간단한 추정치를 사용합니다.
	// AABB의 중심에서 법선 반대 방향으로 침투 깊이를 고려하여 이동한 지점
	Vector2 centerBox = { (min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f };
	info.contactPoint = centerBox + info.normal * (minOverlap);
	// 이는 근사치이며, 정확한 접점은 더 복잡한 알고리즘(GJK-EPA, 클리핑)이 필요합니다.

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
	Vector2 AB = segment.endPoint - segment.startPoint; // 선분 벡터
	float lengthSq = AB.LengthSq(); // 선분 길이의 제곱

	if (lengthSq == 0.0f) // 선분이 점과 같거나 길이가 0인 경우
	{
		t_out = 0.0f;

		return segment.startPoint;
	}

	// 점이 선분 AB에 투영된 위치 (0.0f: p1, 1.0f: p2)
	// t = (point - segment.p1).Dot(AB) / lengthSq;
	float t = Vector2::Dot((point - segment.startPoint), AB) / lengthSq;

	// t 값을 [0, 1] 범위로 클램프하여 선분 내의 가장 가까운 점을 찾도록 보장
	t_out = std::max<float>(0.0f, std::min<float>(1.0f, t));

	// 선분 위의 가장 가까운 점
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

	//// rigidBodyA의 속도 처리
	//if (rigidBodyA && rigidBodyA->GetBodyType() == RigidBody2D::BodyType::Dynamic)
	//{
	//	Vector2 currentVelocityA = rigidBodyA->GetVelocity();
	//	// 현재 속도에서 법선 방향으로의 성분을 계산
	//	Vector2 velocityNormalComponentA = info.normal * Vector2::Dot(currentVelocityA, info.normal);
	//	// 접선 방향 속도 성분만 남김 (법선 방향 속도 제거)
	//	Vector2 velocityTangentComponentA = currentVelocityA - velocityNormalComponentA;
	//	rigidBodyA->SetVelocity(velocityTangentComponentA);
	//}

	//// rigidBodyB의 속도 처리
	//if (rigidBodyB && rigidBodyB->GetBodyType() == RigidBody2D::BodyType::Dynamic)
	//{
	//	Vector2 currentVelocityB = rigidBodyB->GetVelocity();
	//	// 현재 속도에서 법선 방향으로의 성분을 계산
	//	// B는 A로부터 info.normal 방향으로 밀려나는 것이므로, B의 관점에서는 -info.normal 방향으로 충돌함
	//	// 하지만 속도를 분해할 때는 원래의 normal 벡터를 사용해도 됩니다.
	//	// 중요한 것은 B가 info.normal 방향으로 '밀려나는' 것을 방지하는 것입니다.
	//	Vector2 velocityNormalComponentB = info.normal * Vector2::Dot(currentVelocityB, info.normal);
	//	// 접선 방향 속도 성분만 남김 (법선 방향 속도 제거)
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
