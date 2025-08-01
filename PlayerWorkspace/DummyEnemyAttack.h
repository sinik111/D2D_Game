#pragma once

#include "../D2DEngineLib/Script.h"

#include "AttackStateEnum.h"

class DummyEnemyAttack :
	public Script
{
private:
	GameObject* m_debugTextObject = nullptr;
	AttackState m_attackState = AttackState::Searching;
	float m_dummyAttackTimer = 0.0f;
	float m_dummySearchingDuration = 0.4f;
	float m_dummyAttackDuration = 0.2f;
	Vector2 m_debugTextDirection;
	bool m_foundPlayerAttack = false;

private:
	void Start() override;
	void Update() override;

private:
	void OnTriggerEnter(const Collision& collision) override;

public:
	AttackState GetAttackState() const;

	void SetDirection(const Vector2& direction); // 디버그 텍스트 출력용
};