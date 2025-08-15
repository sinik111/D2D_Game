#pragma once

#include "../D2DEngineLib/Script.h"

#include "AttackStateEnum.h"

class Player;
class EnemyBase;
class ConeCollider2D;
class RigidBody2D;


class PlayerGrabAttack :
	public Script
{

public:
	PlayerGrabAttack(float duration = 0.2f)
	{
		m_grabCheckDuration = duration;
	}

private:
	Player* m_player = nullptr;
	GameObject* m_debugTextObject = nullptr;	

	float m_grabCheckDuration = 0.2f;
	float m_grabCheckTimer = 0.0f;
	Vector2 m_debugTextDirection;
	bool m_foundKnockdownEnemy = false;
	
	EnemyBase* m_enemyBase = nullptr;

	ConeCollider2D* m_colliderNormalAttack = nullptr;
	RigidBody2D* m_rigidbodyNormalAttack = nullptr;
		
	bool m_atkEnd = false;

private:
	void Start() override;
	void Update() override;

private:
	void OnTriggerEnter(const Collision& collision) override;

public:	
	Player* GetPlayer() const;

	void SetCollider(ConeCollider2D* col);
	ConeCollider2D* GetCollider();
	void SetRigidBody(RigidBody2D* rb);
	RigidBody2D* GetRigidBody();

	void SetPlayer(Player* player);
	void SetDirection(const Vector2& direction); // 디버그 텍스트 출력용
};

