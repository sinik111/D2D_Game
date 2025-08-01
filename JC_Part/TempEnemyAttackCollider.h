#pragma once
#include "../D2DEngineLib/Script.h"
#include "../D2DEngineLib/FSM.h"
#include "../D2DEngineLib/FSMContext.h"

class EnemyBase;
class RigidBody2D;
class TextRenderer;
class BoxCollider2D;

class TempEnemyAttackCollider : public Script
{

public:
	
	TempEnemyAttackCollider(EnemyBase* enemy = nullptr)
	{
		m_parentEnemy = enemy;
		m_rigidBody = nullptr;
		m_textRenderer = nullptr;
		m_collider = nullptr;
	}

private:

	EnemyBase* m_parentEnemy;

	RigidBody2D* m_rigidBody;
	TextRenderer* m_textRenderer;
	BoxCollider2D* m_collider;

	FSM m_fsm;
	FSMContext m_context;

	bool m_isParringTiming;

	float m_parringEffectTime;

private:
	void Initialize() override;
	void Start() override;
	void FixedUpdate() override;
	void Update() override;
	void OnCollisionEnter(const Collision& collision) override;
	void OnCollisionStay(const Collision& collision) override;
	void OnCollisionExit(const Collision& collision) override;

	void OnTriggerEnter(const Collision& collision) override;
	void OnTriggerExit(const Collision& collision) override;


};

