#pragma once

#include "../D2DEngineLib/Script.h"

#include "AttackStateEnum.h"

class Player;
class EnemyBase;
class EnemyBaseAttack;
class ConeCollider2D;
class RigidBody2D;

class PlayerNormalAttack :
	public Script
{

public:	
	PlayerNormalAttack(Player* player = nullptr, float duration = 0.0f, float coneRadius = 100.0f, float coneAngle  = 30.0f)
	{
		m_player = player;
		m_totalAttackDuration = duration;
		m_coneRadius = coneRadius;
		m_coneAngle = coneAngle;
	}

private:
	Player* m_player = nullptr;
	GameObject* m_debugTextObject = nullptr;
	AttackState m_attackState = AttackState::Searching;

	float m_coneRadius = 100.0f;
	float m_coneAngle = 30.0f;
		
	float m_attackTimer = 0.0f;
	float m_searchingDuration = 0.4f;
	float m_attackDuration = 0.2f;
	Vector2 m_debugTextDirection;
	bool m_foundEnemyAttack = false;

	bool m_enemyHitChecked = false;
	EnemyBase* m_enemyBase = nullptr;

	ConeCollider2D* m_colliderNormalAttack = nullptr;
	RigidBody2D* m_rigidbodyNormalAttack = nullptr;

	float m_totalAttackDuration = 1.0f;
	bool m_atkEnd = false;

private:
	void Start() override;
	void Update() override;

private:
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
};