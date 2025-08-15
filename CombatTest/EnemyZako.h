#pragma once
#include "EnemyBase.h"

class EnemyZako : public EnemyBase
{
public:
	EnemyZako()
	{
		m_enemyType = ZAKO;		
	}
protected:
	void Initialize() override;	
	void Start() override;

	void OnCollisionEnter(const Collision& collision) override;
	void OnCollisionStay(const Collision& collision) override;
	void OnCollisionExit(const Collision& collision) override;

	void OnTriggerEnter(const Collision& collision) override;
	void OnTriggerExit(const Collision& collision) override;

	void AnimationInit() override;
	void ColliderInit()override;	

	void StateInit() override;
	void DebugTextInit() override;
	void DebugTextUpdate() override;
	void vSetting() override;

	void SetAnimState(int animState) override;


	void CheckAndTransitState() override;
	void CheckAndTransitAnimState() override;

	
};

