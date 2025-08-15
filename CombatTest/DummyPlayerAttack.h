#pragma once
#include "../D2DEngineLib/FSM.h"
#include "../D2DEngineLib/FSMContext.h"
#include "../D2DEngineLib/Script.h"

#include "AttackStateEnum.h"

class DummyP;

class DummyPlayerAttack :
	public Script
{

public:
	DummyPlayerAttack(DummyP* player = nullptr)
	{
		m_player = player;
	}

private:

	DummyP* m_player = nullptr;

	GameObject* m_debugTextObject = nullptr;
	AttackState m_attackState = AttackState::Searching;
	float m_dummyAttackTimer = 0.0f;
	float m_dummySearchingDuration = 0.4f;
	float m_dummyAttackDuration = 0.2f;
	Vector2 m_debugTextDirection;
	bool m_foundEnemyAttack = false;

private:
	void Start() override;
	void Update() override;

private:
	void OnCollisionEnter(const Collision& collision) override;
	void OnTriggerEnter(const Collision& collision) override;

	void HitEnemy(const Collision& collision);

public:
	AttackState GetAttackState() const;

	void SetTextDirection(const Vector2& direction); // 디버그 텍스트 출력용


};


