#pragma once
#include "../D2DEngineLib/Script.h"
#include "../D2DEngineLib/FSM.h"
#include "../D2DEngineLib/FSMContext.h"

#include "AttackStateEnum.h"

class EnemyBase;


class TempEnemyAttack : public Script
{

public:
	
	TempEnemyAttack(EnemyBase* enemy = nullptr)
	{
		m_parentEnemy = enemy;	
	}

private:

	//FSM m_fsm;
	//FSMContext m_context;

	EnemyBase* m_parentEnemy;
	
	GameObject* m_debugTextObject = nullptr;

	AttackState m_attackState = AttackState::Safety;	

	float m_dummyAttackTimer = 0.0f;

	float m_dummySafetyDuration = 0.3f;		//������ ���� 0.3��
	float m_dummySearchingDuration = 0.5f;	//���� ���� ���� 0.5��	
	float m_dummyAttackDuration = 0.2f;		//���� �ĵ� 0.2��
	
	Vector2 m_debugTextDirection;
	bool m_foundPlayerAttack = false;

	bool m_isParringTime;
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

public:
	AttackState GetAttackState() const;

	void SetTextDirection(const Vector2& direction);


};

