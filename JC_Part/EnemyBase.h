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
	TextRenderer* m_textRenderer;
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
	std::wstring animPath[10] = {
		L"",
		L"",
		L"",
		L""
		L"",
		L"",
		L"",
		L"",
		L"",
		L"",
	};

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
		NOSTATE,
		OUTOFAREA,		//카메라 범위 밖	
		INAREA,			//카메라 범위 안
		ENGAGE,			//이동 가능한 위치에 있는 플레이어 발견
		ONATTACK,
		RETURN,
	};

	GameObject* m_camera;
	bool m_isInCamera;

	bool m_isEngage;

	bool m_isMove;

	int m_nextEnemyState;
	int m_currEnemyState;
	int m_prevEnemyState;

	float m_rotationAngle = 0.0f;
	int m_direction = DOWN;

	int m_prevDir;
	int m_currDir;

	GameObject* m_player;
	Vector2 m_originPos;
	float m_originAngle;

	bool m_toDoMove;
	Vector2 m_movingDestPos;

	float m_moveSpeed;
	float m_maxSightDistance;
	float m_sightAngle;
	float m_maxRoamDistance;
	float m_maxChaseDistance;

	void CheckState();

	void SetTargetPlayerAndCamera(GameObject* player, GameObject* camera);

	void PositionInit(float x, float y, float angle);

	void MoveTo(const Vector2& destination);

	void StopMoving();

	void SetAnimDirection();

	void UpdateDirection();

	void SetRotationAngle(float angle);

	void SetDirection(int n);


	//void SetMovingDestPos();

	//void RandomDirPatrol();

	//void LockOnPlayer();

	//void AttackRangeCheck();

	//void AttackRandom();

	//void AttackTypeA();
	//void AttackTypeB();
	//void AttackTypeC();

	//void ReturnToOrigin();


public:

	inline int& CurrEnemyState() { return m_currEnemyState; }

	inline int& PrevEnemyState() { return m_prevEnemyState; }	

	inline bool& IsMove() { return m_isMove; }	

	inline float& RotationAngle() { return m_rotationAngle; }

	inline int& PrevDir() { return m_prevDir; }

	inline int& CurrDir() { return m_currDir; }

	inline float& OriginAngle() { return m_originAngle; }

	inline float& MoveSpeed() {	return m_moveSpeed; }

	inline float& MaxSightDistance() { return m_maxSightDistance; }

	inline float& SightAngle() { return m_sightAngle; }

	inline float& MaxRoamDistance() { return m_maxRoamDistance; }

	inline float& MaxChaseDistance() { return m_maxChaseDistance; }

	inline int& Direction() { return m_direction; }

	inline const Vector2& CameraPos() { return m_camera->GetTransform()->GetLocalPosition(); }

	inline bool& IsInCamera() { return m_isInCamera; }

	inline const Vector2& TargetPos() { return m_player->GetTransform()->GetLocalPosition(); }

	inline Vector2& OriginPos() { return m_originPos; }

	inline Vector2& MovingDestPos() { return m_movingDestPos; }

	inline bool& ToDoMove() { return m_toDoMove; }

	RigidBody2D* RigidBody();

	const Vector2& Pos();

};

