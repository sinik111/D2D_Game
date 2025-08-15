
#pragma once
#include "EnemyBase.h"

class EnemyBoss : public EnemyBase
{
public:
	EnemyBoss()
	{
		m_enemyType = BOSS;
	}
protected:
	void Initialize() override;
	void Start() override;
	void Update() override;

	void OnCollisionEnter(const Collision& collision) override;
	void OnCollisionStay(const Collision& collision) override;
	void OnCollisionExit(const Collision& collision) override;

	void OnTriggerEnter(const Collision& collision) override;
	void OnTriggerExit(const Collision& collision) override;

	void AnimationInit() override;
	void ColliderInit()override;
	void InitAnimPath(AnimPathArray& animPath, const std::wstring& animName) override;

	void StateInit() override;
	void DebugTextInit() override;
	void DebugTextUpdate() override;
	void vSetting() override;

	void SetAnimState(int animState) override;


	void CheckAndTransitState() override;
	void CheckAndTransitAnimState() override;

	AnimPathArray m_bossDashAnimPath;
	AnimPathArray m_bossMoveInAnimPath;
	AnimPathArray m_bossMoveOutAnimPath;


	// Animation Events
public:
	void Footstep1();
	void Footstep2();
	void HAttack1();
	void HAttack2();
	void HAttack3();
	void Idle();
	void SAttacking();
	void SPreAttacking();
	void Dead();
};

