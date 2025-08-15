#pragma once

#include "../D2DEngineLib/Script.h"

#include "AttackStateEnum.h"

class Player;
class EnemyBase;
class ConeCollider2D;
class RigidBody2D;

class PlayerHeavyAttack :
	public Script
{
public:
	PlayerHeavyAttack()
	{
		m_player = nullptr;
		m_totalAttackDuration = 0.0f;
		m_coneRadius = 100.0f;
		m_coneAngle = 30.0f;

		m_safetyDuration = 0.2f;
		m_endDuration = 0.48f;
	}

	PlayerHeavyAttack(Player* player = nullptr, float duration = 1.0f, float coneRadius = 100.0f, float coneAngle = 30.0f,
		float safetyDuration = 0.2f, float endDuration = 0.48f)
	{
		m_player = player;
		m_totalAttackDuration = duration;
		m_coneRadius = coneRadius;
		m_coneAngle = coneAngle;

		m_safetyDuration = safetyDuration;
		m_endDuration = endDuration;
	}

private:
	Player* m_player = nullptr;	
	AttackState m_attackState = AttackState::Safety;

	float m_coneRadius = 100.0f;
	float m_coneAngle = 30.0f;

	float m_attackTimer = 0.0f;
	float m_hitCheckTimer = 0.0f;

	float m_safetyDuration = 0.2f;
	float m_endDuration = 0.48f;
	float m_totalAttackDuration = 1.0f;

	Vector2 m_debugTextDirection;

	bool m_isEffective = false;

	bool m_enemyHitChecked = false;
	bool m_parringChecked = false;

	float m_moveReadyDuration = 0.2f;

	ConeCollider2D* m_colliderHeavyAttack = nullptr;
	RigidBody2D* m_rigidbodyHeavyAttack = nullptr;

	bool m_atkEnd = false;

private:
	void Initialize() override;
	void Start() override;
	void Update() override;

private:
	void OnTriggerStay(const Collision& collision) override;
	void OnTriggerEnter(const Collision& collision) override;

public:
	AttackState GetAttackState() const;
	Player* GetPlayer() const;

	void SetCollider(ConeCollider2D* col);
	ConeCollider2D* GetCollider();
	void SetRigidBody(RigidBody2D* rb);
	RigidBody2D* GetRigidBody();

	void SetPlayer(Player* player);
	void SetDirection(const Vector2& direction); // 디버그 텍스트 출력용
	void ColliderMoveUpdate();

public:

	Vector2 m_startPosition; // 공격 오브젝트의 시작 위치
	Vector2 m_endPosition;   // 공격 오브젝트의 목표 도착 위치
	Vector2 m_direction;

	float m_moveTimer = 0.0f; // 이동 시간 누적 타이머
	float m_moveDuration = 0.0f; // 총 이동에 걸리는 시간
	bool m_isMoving = false;

	Vector2 AssaultStartPos = Vector2::Zero;
	Vector2 AssaultEndPos = Vector2::Zero;
	void AssaultMove();

	bool m_isOnAMove = false;
	float m_aMoveTimer = 0.0f;
	float m_aMoveDuration = 0.3f;

};

