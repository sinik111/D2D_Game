#pragma once

#include "../D2DEngineLib/Script.h"

#include "AttackStateEnum.h"

class Player;
class EnemyBase;
class ConeCollider2D;
class RigidBody2D;


class PlayerKnockdownCheck :
	public Script
{

public:
	PlayerKnockdownCheck(Player* player = nullptr, float coneRadius = 250.0f, float coneAngle = 50.0f)
	{
		m_player = player;
		m_grabCheckDuration = 0.2f;
		m_coneRadius = coneRadius;
		m_coneAngle = coneAngle;
	}

private:
	Player* m_player = nullptr;	

	float m_grabCheckDuration = 0.2f;
	float m_grabCheckTimer = 0.0f;
	Vector2 m_debugTextDirection;

	float m_coneRadius = 250.0f;
	float m_coneAngle = 50.0f;
	
	std::vector<EnemyBase*> m_knockdownEnemies;

	EnemyBase* m_enemyBase = nullptr; 

	ConeCollider2D* m_colliderCheckKnockdown = nullptr;
	RigidBody2D* m_rigidbodyCheckKnockdown = nullptr;

	bool m_atkEnd = false;


private:
	void Initialize() override;
	void Start() override;
	void Update() override;

private:
	void OnTriggerEnter(const Collision& collision) override;
	void FindClosestTarget();

public:	
	Player* GetPlayer() const;

	void SetCollider(ConeCollider2D* col);
	ConeCollider2D* GetCollider();
	void SetRigidBody(RigidBody2D* rb);
	RigidBody2D* GetRigidBody();

	void SetPlayer(Player* player);
	void SetDirection(const Vector2& direction); // 디버그 텍스트 출력용
};

