#pragma once
#include "../D2DEngineLib/IState.h"
#include "EnemyBase.h"


class EnemyState
	: public IState
{
public:

	EnemyState() {}	

	EnemyBase* m_Script;

	void SetScript(EnemyBase* script)
	{
		m_Script = script;
	}

	void Enter(FSMContext& context) override;

	void Update(FSMContext& context) override;

	void Exit(FSMContext& context) override;	
		

	inline bool& IsMove() { return m_Script->IsMove(); }

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

	inline int& Direction() { return m_Script->Direction(); }

	inline const Vector2& CameraPos() { return m_Script->CameraPos(); }

	inline bool& IsInCamera() {	return m_Script->IsInCamera(); }

	inline const Vector2& TargetPos() { return m_Script->TargetPos(); }

	inline Vector2& OriginPos() { return m_Script->OriginPos(); }

	inline Vector2& MovingDestPos() { return m_Script->MovingDestPos(); }

	inline bool& ToDoMove() { return m_Script->ToDoMove(); }

	RigidBody2D* RigidBody(); 
	

	void CheckCameraArea();

	void SetEnemyDirectionByInput(FSMContext& context);

	void StopMoving();

	void UpdateDirection();

	void SetAngle(float angle);

	void SetDirection(int n);


	virtual void UpdateMovement(FSMContext& context);


	inline float LerpAngle(float a, float b, float t)
	{
		// 각도 보간 로직 (예: -180 ~ 180 또는 0 ~ 360 범위 처리)
		// 가장 일반적인 각도 Lerp는 다음과 같은 형태를 가집니다.
		float delta = fmod(b - a, 360.0f);
		if (delta > 180.0f) delta -= 360.0f;
		if (delta < -180.0f) delta += 360.0f;
		return a + delta * t;
	}

	enum Dir	//Num키 기준으로 배열. 0은 랜덤(나중에 쓰게 된다면..), 5는 NONE(값 없음)
	{
		RANDOM,			//0
		LEFT_DOWN,		//1
		DOWN,			//2
		RIGHT_DOWN,		//3
		LEFT,			//4
		NONE,			//5
		RIGHT,			//6
		LEFT_UP,		//7	
		UP,				//8
		RIGHT_UP,		//9	
	};


	enum State
	{
		OUTOFAREA,		//카메라 범위 밖	
		INAREA,			//카메라 범위 안
		ENGAGE,			//이동 가능한 위치에 있는 플레이어 발견
		ONATTACK,
		RETURN,
	};
};




