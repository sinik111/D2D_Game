#pragma once

#include "../D2DEngineLib/Script.h"
#include "../D2DEngineLib/FSM.h"
#include "../D2DEngineLib/FSMContext.h"

class PlayerInput;
class Animator;
class RigidBody2D;
class TextRenderer;
class BoxCollider2D;


class EnemyBase :public Script
{
private:
	PlayerInput* m_playerInput;
	Animator* m_animator;	
	RigidBody2D* m_rigidBody;
	TextRenderer* m_text;
	BoxCollider2D* m_collider;

	FSM m_fsm;
	FSMContext m_context;	
	

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


	enum Condition
	{		
		OUTOFAREA,		//카메라 범위 밖	
		INAREA,			//카메라 범위 안
		ENGAGE,			//이동 가능한 위치에 있는 플레이어 발견
		ONATTACK,
		RETURN,		
	};

	float m_rotationAngle = 0.0f;
	int m_direction = DOWN;
		

	GameObject* m_camera;
	GameObject* m_player;
	Vector2 m_originPosition;
	float m_originAngle;	

	bool m_toDoMove;
	Vector2 m_movingDestPos;

	float m_moveSpeed;
	float m_maxSightDistance;
	float m_sightAngle;
	float m_maxRoamDistance;
	float m_maxChaseDistance;

	bool m_isInCamera;
	bool m_isEngage;


	void SetEnemyDirectionByInput();
	void SetEnemyDirectionByRotation(float angle);

	void SetTargetPlayerAndCamera(GameObject* player, GameObject* camera);

	void PositionInit(float x, float y, float angle);

	void CheckCameraArea();

	void SetMovingDestPos();

	void RandomDirPatrol();

	void LockOnPlayer();

	void AttackRangeCheck();

	void AttackRandom();

	void AttackTypeA();
	void AttackTypeB();
	void AttackTypeC();

	void ReturnToOrigin();
	
	


};

