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
		// ���� ���� ���� (��: -180 ~ 180 �Ǵ� 0 ~ 360 ���� ó��)
		// ���� �Ϲ����� ���� Lerp�� ������ ���� ���¸� �����ϴ�.
		float delta = fmod(b - a, 360.0f);
		if (delta > 180.0f) delta -= 360.0f;
		if (delta < -180.0f) delta += 360.0f;
		return a + delta * t;
	}

	enum Dir	//NumŰ �������� �迭. 0�� ����(���߿� ���� �ȴٸ�..), 5�� NONE(�� ����)
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
		OUTOFAREA,		//ī�޶� ���� ��	
		INAREA,			//ī�޶� ���� ��
		ENGAGE,			//�̵� ������ ��ġ�� �ִ� �÷��̾� �߰�
		ONATTACK,
		RETURN,
	};
};




