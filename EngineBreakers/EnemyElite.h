#pragma once
#include "EnemyBase.h"

class EnemyElite : public EnemyBase
{
public:
	EnemyElite()
	{
		m_enemyType = ELITE;
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

	void EnemyStabAttack();
	void EnemyAttack();
};

