#pragma once

#include "../D2DEngineLib/Script.h"
#include "../D2DEngineLib/FSM.h"
#include "../D2DEngineLib/FSMContext.h"

class Animator;
class Health;
class RigidBody2D;

class EnemyBase :public Script
{
private:
	Animator* m_animator;
	
	RigidBody2D* m_rigidBody;

	FSM m_fsm;
	FSMContext m_context;

	bool m_isJump = false;
	bool m_isGround = false;

	int direction = DOWN;


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

private:

	void ArrowInput(Vector2 input);	


public:
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

	void SetEnemyDirectionByInput();
};

