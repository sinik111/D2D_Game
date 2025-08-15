#pragma once
#include "../D2DEngineLib/IState.h"
#include "EnemyBase.h"

using namespace std;


class EnemyState
	: public IState
{
public:

	EnemyState() {}	

	EnemyBase* m_Script = nullptr;

	void SetScript(EnemyBase* script)
	{
		m_Script = script;
	}

	void Enter(FSMContext& context) override;

	void Update(FSMContext& context) override;

	void Exit(FSMContext& context) override;	

	float m_dTime = 0.0f;

	bool m_isStateChanged = false;
	

	inline const Vector2& Pos() { return m_Script->Pos(); }

	inline float& RotationAngle() { return m_Script->RotationAngle(); }

	inline int& PrevDir() { return m_Script->PrevDir(); }

	inline int& CurrDir() { return m_Script->CurrDir(); }

	inline float& OriginAngle() { return m_Script->OriginAngle(); }

	inline float& MoveSpeed() { return m_Script->MoveSpeed(); }

	inline float& MaxSightDistance() { return m_Script->MaxSightDistance(); }

	inline float& SightAngle() { return m_Script->SightAngle(); }

	inline float& MaxRoamDistance() { return m_Script->MaxRoamDistance(); }

	inline float& MaxChaseDistance() { return m_Script->MaxChaseDistance(); }

	inline float& AttackRange() { return m_Script->AttackRange(); }

	inline int& Direction() { return m_Script->Direction(); }

	inline const Vector2& CameraPos() { return m_Script->CameraPos(); }

	inline bool& IsInCamera() {	return m_Script->IsInCamera(); }

	inline const Vector2& TargetPos() { return m_Script->TargetPos(); }

	inline Vector2& OriginPos() { return m_Script->OriginPos(); }

	inline Vector2& MovingDestPos() { return m_Script->MovingDestPos(); }

	inline bool& ToDoMove() { return m_Script->ToDoMove(); }

	inline bool& IsLockOnTarget() { return m_Script->IsLockOnTarget(); }

	inline bool& IsTargetInChaseDist() { return m_Script->IsTargetInChaseDist(); }

	inline bool& IsTargetInRoamDist() { return m_Script->IsTargetInRoamDist(); }	

	inline bool& IsTargetInAtkRange() { return m_Script->IsTargetInAtkRange(); }

	inline bool& IsTargetInMaxAtkRange() { return m_Script->IsTargetInMaxAtkRange(); }

	inline float& AtkAngle() { return m_Script->AtkAngle(); }

	inline bool& IsTargetInAtkAngle() { return m_Script->IsTargetInAtkAngle(); }

	inline bool& IsAttackReady() { return m_Script->IsAttackReady(); }

	inline bool& IsUnderAttack() { return m_Script->IsUnderAttack(); }

	inline bool& IsFindPlayer() { return m_Script->IsFindPlayer(); }
		
	RigidBody2D* RigidBody(); 
	
	inline const bool IsPlayerNull() { return m_Script->IsPlayerNull(); }

	inline bool& IsKnockback() { return m_Script->IsKnockback(); }
	
	inline bool& IsKnockdown() { return m_Script->IsKnockdown(); }
	
	

	const bool& IsPlayerDead(FSMContext& context);		
		
	void CheckTargetDistance();	

	void MoveToPlayer();

	void CheckTargetInAtkRange();

	void CheckTargetInAtkAngle();

	void CheckKnockdown(FSMContext& context);

	
	

	//void SetEnemyDirectionByInput(FSMContext& context);
	


public:	

	inline float LerpAngle(float a, float b, float t)
	{
		// 각도 보간 로직 (예: -180 ~ 180 또는 0 ~ 360 범위 처리)
		// 가장 일반적인 각도 Lerp는 다음과 같은 형태를 가집니다.
		float delta = fmod(b - a, 360.0f);
		if (delta > 180.0f) delta -= 360.0f;
		if (delta < -180.0f) delta += 360.0f;
		return a + delta * t;
	}
	
};