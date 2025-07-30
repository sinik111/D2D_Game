#pragma once

#include "../D2DEngineLib/Script.h"
#include "../D2DEngineLib/FSM.h"
#include "../D2DEngineLib/FSMContext.h"

class Animator;
class BitmapRenderer;
class RigidBody2D;
class TextRenderer;
class PlayerInput;

class DummyP : public Script
{
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
		
	
	PlayerInput* m_playerInput;
	
	//Animator* m_animator;
	BitmapRenderer* m_bitmapRenderer;

	RigidBody2D* m_rigidBody;
	TextRenderer* m_text;

	//BoxCollider2D* m_collider;

	FSM m_fsm;
	FSMContext m_context;	

	float rotationAngle = 0.0f;
	int direction = DOWN;


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
		
	void SetDirectionByRotation(float angle);

	void Attack();
	

};


