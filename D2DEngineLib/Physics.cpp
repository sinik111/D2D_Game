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
		if (distance < MyMath::EPSILON)
		{
			info.normal = { 0.0f, 1.0f };
		}
		else
		{
			info.normal = displacement.Normalized(); // circleA에서 circleB로 향하는 단위 벡터
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

		if (axis.LengthSq() < MyMath::EPSILON)
		{
			continue;
		}

		axis = axis.Normalized(); // 단위 벡터로 정규화	//zeroDivide 발생한 적 있음

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
		if (distance < MyMath::EPSILON)
		{
			Vector2 aabbCenter = { (boxMin.x + boxMax.x) / 2.0f, (boxMin.y + boxMax.y) / 2.0f };
			Vector2 direction = circle.center - aabbCenter;
			
			if (direction.LengthSq() < MyMath::EPSILON) // 만약 원의 중심이 AABB 중심과 겹치면
			{
				info.normal = { 0.0f, 1.0f }; // 기본값으로 위쪽 법선 설정
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
				info.normal = { 0.0f, 1.0f }; // 기본값으로 위쪽 법선 설정
			}
			else
			{
				info.normal = displacement.Normalized(); // 원 중심에서 closestPoint로 향하는 벡터의 정규화
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
		if (distance < MyMath::EPSILON)
		{
			Vector2 aabbCenter = { (boxMin.x + boxMax.x) / 2.0f, (boxMin.y + boxMax.y) / 2.0f };
			Vector2 direction = circle.center - aabbCenter;

			if (direction.LengthSq() < MyMath::EPSILON) // 만약 원의 중심이 AABB 중심과 겹치면
			{
				info.normal = { 0.0f, 1.0f }; // 기본값으로 위쪽 법선 설정
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
				info.normal = { 0.0f, 1.0f }; // 기본값으로 위쪽 법선 설정
			}
			else
			{
				info.normal = -displacement.Normalized(); // 원 중심에서 closestPoint로 향하는 벡터의 정규화
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

	float t; // 선분 위에서 가장 가까운 점의 비율
	Vector2 closestPoint = GetClosestPointOnLineSegment(circle.center, segment, t);

	// 원의 중심에서 선분 위의 가장 가까운 점으로의 벡터
	Vector2 distVec = circle.center - closestPoint;
	float distanceSq = distVec.LengthSq();
	float radiusSq = circle.radius * circle.radius;

	// 거리가 반지름보다 작으면 충돌
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

	float t; // 선분 위에서 가장 가까운 점의 비율
	Vector2 closestPoint = GetClosestPointOnLineSegment(circle.center, segment, t);

	// 원의 중심에서 선분 위의 가장 가까운 점으로의 벡터
	Vector2 distVec = circle.center - closestPoint;
	float distanceSq = distVec.LengthSq();
	float radiusSq = circle.radius * circle.radius;

	// 거리가 반지름보다 작으면 충돌
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

	axes.push_back(Vector2(-(coneVertices[1].y - coneVertices[0].y), coneVertices[1].x - coneVertices[0].x).Normalized());
	axes.push_back(Vector2(-(coneVertices[2].y - coneVertices[1].y), coneVertices[2].x - coneVertices[1].x).Normalized());
	axes.push_back(Vector2(-(coneVertices[0].y - coneVertices[2].y), coneVertices[0].x - coneVertices[2].x).Normalized());

	for (const auto& axis : axes)
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

		const float currentOverlap = projBox.GetOverlap(projCone);

		// 최소 겹침 업데이트
		if (currentOverlap < minOverlap)
		{
			minOverlap = currentOverlap;
			collisionNormal = axis;

			Vector2 centerBox{ (min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f };
			Vector2 centerCone = cone.center;
			Vector2 dirBoxToCone = centerCone - centerBox;

			if (Vector2::Dot(collisionNormal, dirBoxToCone) < 0)
			{
				collisionNormal = collisionNormal * -1.0f; // 법선 방향 뒤집기
			}
		}
	}

	info.isCollide = true;
	info.penetrationDepth = minOverlap;
	info.normal = collisionNormal.Normalized(); // 최종 법선은 단위 벡터

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

	axes.push_back({ 1.0f, 0.0f }); // X축
	axes.push_back({ 0.0f, 1.0f }); // Y축

	axes.push_back(Vector2(-(coneVertices[1].y - coneVertices[0].y), coneVertices[1].x - coneVertices[0].x).Normalized());
	axes.push_back(Vector2(-(coneVertices[2].y - coneVertices[1].y), coneVertices[2].x - coneVertices[1].x).Normalized());
	axes.push_back(Vector2(-(coneVertices[0].y - coneVertices[2].y), coneVertices[0].x - coneVertices[2].x).Normalized());

	for (const auto& axis : axes)
	{
		const Projection projBox = ProjectPolygon(boxVertices, 4, axis); // 박스는 4개의 정점
		const Projection projCone = ProjectPolygon(coneVertices, 3, axis); // 콘은 3개의 정점

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

			Vector2 centerBox{ (min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f };
			Vector2 centerCone = cone.center;
			Vector2 dirBoxToCone = centerCone - centerBox;

			if (Vector2::Dot(collisionNormal, dirBoxToCone) < 0)
			{
				collisionNormal = collisionNormal * -1.0f; // 법선 방향 뒤집기
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
	Vector2 AB = segment.endPoint - segment.startPoint; // 선분 벡터
	float lengthSq = AB.LengthSq(); // 선분 길이의 제곱

	if (lengthSq < MyMath::EPSILON) // 선분이 점과 같거나 길이가 0인 경우
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
