#pragma once

#include "../D2DEngineLib/Script.h"
#include "../D2DEngineLib/FSM.h"
#include "../D2DEngineLib/FSMContext.h"

class PlayerInput;
class Animator;
//class RigidBody2D;
//class TextRenderer;

class EnemyBase :public Script
{
private:

	PlayerInput* m_playerInput;

	Animator* m_animator;
	
	//RigidBody2D* m_rigidBody;

	//TextRenderer* m_text;

	FSM m_fsm;
	FSMContext m_context;	

	bool m_isJump = false;
	bool m_isGround = false;

	float degree = 0.0f;
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

	void SetEnemyDirectionByInput();
};

