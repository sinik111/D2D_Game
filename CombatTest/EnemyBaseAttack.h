#pragma once
#include "../D2DEngineLib/Script.h"
#include "../D2DEngineLib/FSM.h"
#include "../D2DEngineLib/FSMContext.h"

#include "AttackStateEnum.h"

class EnemyBase;

class EnemyBaseAttack : public Script
{
public:
	EnemyBaseAttack(EnemyBase* enemy = nullptr)
	{
		m_parentEnemy = enemy;
	}

private:
	EnemyBase* m_parentEnemy;

	GameObject* m_debugTextObject = nullptr;
	AttackState m_attackState = AttackState::Safety;
	float m_dummyAttackTimer = 0.0f;
	float m_dummySafetyDuration = 0.3f;		//공격의 전조 0.3초
	float m_dummySearchingDuration = 0.5f;	//실제 공격 판정 0.5초	
	float m_dummyAttackDuration = 0.2f;		//공격 후딜 0.2초

	Vector2 m_debugTextDirection;
	bool m_foundPlayerAttack = false;

	bool m_isParringTime;
	float m_parringEffectTime;

private:

	void Start() override;
	void Update() override;

	void OnTriggerEnter(const Collision& collision) override;
	void OnTriggerStay(const Collision& collision) override;

public:
	AttackState GetAttackState() const;
	EnemyBase* GetEnemyBase() const;
	void SetAttackState(AttackState state);

	void SetDirection(const Vector2& direction);

public:
	ConeCollider2D* m_colliderEnemyAttack;
	RigidBody2D* m_rigidbodyEnemyAttack;

	void SetCollider(ConeCollider2D* col);
	ConeCollider2D* GetCollider();
	void SetRigidBody(RigidBody2D* rb);
	RigidBody2D* GetRigidBody();

	

};

