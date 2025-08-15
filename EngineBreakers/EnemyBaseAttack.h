#pragma once
#include "../D2DEngineLib/Script.h"
#include "../D2DEngineLib/FSM.h"
#include "../D2DEngineLib/FSMContext.h"

#include "AttackStateEnum.h"

class Player;
class EnemyBase;
class ConeCollider2D;
class RigidBody2D;


class EnemyBaseAttack : public Script
{
public:
	EnemyBaseAttack(EnemyBase* enemy = nullptr, float duration = 1.0f, float coneRadius = 100.0f, float coneAngle = 30.0f,
		float safetyDuration = 0.4f, float endDuration = 0.87f, int dmg = 0, int knock = 0)
	{
		m_parentEnemy = enemy;
		m_totalAttackDuration = duration;
		m_coneRadius = coneRadius;
		m_coneAngle = coneAngle;

		m_safetyDuration = safetyDuration;
		m_endDuration = endDuration;

		m_dmg = dmg;
		m_knock = knock;
	}


private:
	EnemyBase* m_parentEnemy;

	//GameObject* m_debugTextObject = nullptr;
	AttackState m_attackState = AttackState::Safety;

	float m_coneRadius = 100.0f;
	float m_coneAngle = 30.0f;

	float m_attackTimer = 0.0f;
	float m_hitCheckTimer = 0.0f;

	float m_safetyDuration = 0.4f;
	float m_endDuration = 0.87f;
	float m_totalAttackDuration = 1.0f;

	int m_dmg = 0;
	int m_knock = 0;

	Vector2 m_debugTextDirection;
	Vector2 m_enemyAttackDirection;

	//bool m_isEffective = false;

	bool m_playerHitChecked = false;
	

	ConeCollider2D* m_colliderNormalAttack = nullptr;
	RigidBody2D* m_rigidbodyNormalAttack = nullptr;

	bool m_atkEnd = false;

public:

	bool m_isColliderChecked = false;

	void SetChecked()
	{
		m_isColliderChecked = true;
	}
	bool GetChecked()
	{
		return m_isColliderChecked;
	}
	bool m_isJustTime = false;

	bool GetIsJustTime() {
		if (m_parentEnemy->IsDead() == false)
			return m_isJustTime;
		else
			return false;
	}
	
	bool m_isNoDmg = false;
	
	void SetIsNoDmg()
	{
		if (m_parentEnemy->IsDead() == false)
		{
			m_parentEnemy->SetIsGetParried();
		}
		m_isNoDmg = true;
	}

	void SetIsNoDmgHeavy()
	{
		if (m_parentEnemy->IsDead() == false)
		{
			m_parentEnemy->SetIsGetParriedHeavy();
		}
		m_isNoDmg = true;
	}

	Vector2 m_startPosition; // 공격 오브젝트의 시작 위치
	Vector2 m_endPosition;   // 공격 오브젝트의 목표 도착 위치
	Vector2 m_direction;

	float m_moveTimer = 0.0f; // 이동 시간 누적 타이머
	float m_moveDuration = 0.0f; // 총 이동에 걸리는 시간
	bool m_isMoving = false;


private:
	void Initialize() override;
	void Start() override;
	void Update() override;

	void OnTriggerEnter(const Collision& collision) override;
	void OnTriggerStay(const Collision& collision) override;

public:
	AttackState GetAttackState() const;
	EnemyBase* GetEnemyBase() const;
	void SetAttackState(AttackState state);

	void SetDirection(const Vector2& direction);

	void ColliderMoveUpdate();

public:
	ConeCollider2D* m_colliderEnemyAttack = nullptr;
	RigidBody2D* m_rigidbodyEnemyAttack = nullptr;

	void SetCollider(ConeCollider2D* col);
	ConeCollider2D* GetCollider();
	void SetRigidBody(RigidBody2D* rb);
	RigidBody2D* GetRigidBody();

};

