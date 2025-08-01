#pragma once

#include "../D2DEngineLib/Script.h"
#include "../D2DEngineLib/FSM.h"
#include "../D2DEngineLib/FSMContext.h"

class PlayerInput;
class Animator;
class RigidBody2D;
class TextRenderer;
class BoxCollider2D;

class TempEnemyAttackCollider;

class EnemyBase :public Script
{
private:
	PlayerInput* m_playerInput;
	Animator* m_animator;
	RigidBody2D* m_rigidBody;
	TextRenderer* m_textRenderer;
	//BoxCollider2D* m_collider;

	FSM m_fsm;
	FSMContext m_context;

	GameObject* m_atkCollider = nullptr;

	std::wstring m_tempLabel = L"EnemyAttackCollider_1";

	float m_debugLogTimer = 1.0f;
	float m_dTimer = 0.0f;


private:
	void Initialize() override;
	void Start() override;
	void FixedUpdate() override;
	void Update() override;
	void OnDestroy() override;

	void OnCollisionEnter(const Collision& collision) override;
	void OnCollisionStay(const Collision& collision) override;
	void OnCollisionExit(const Collision& collision) override;

	void OnTriggerEnter(const Collision& collision) override;
	void OnTriggerExit(const Collision& collision) override;

private:
	void ArrowInput(Vector2 input);
	void vSetting();

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

	//---------------------------------------------------------------------------------
	//int AttackPowerMin;
	//int AttackPowerMax;
	//int	Attack2PowerMin;
	//int Attack2PowerMax;
	//int Attack3PowerMin;
	//int Attack3PowerMax;
	//int AttackKnockdownPower;	

	//int KnockbackResist;
	//int KnockdownResist;
	//float KnockdownPointResetTime;

	//float EvadeDistance;
	//float EvadeProbability;
	//float Attack1Probability;
	//float Attack2Probability;
	//float Attack3Probability;
	//---------------------------------------------------------------------------------

	bool m_isDead = false;

	GameObject* m_camera;
	bool m_isInCamera = false;

	int m_nextEnemyState;
	int m_currEnemyState;
	int m_prevEnemyState;

	float m_rotationAngle = 0.0f;
	int m_direction = DOWN;

	int m_prevDir;
	int m_currDir;

	GameObject* m_player;
	bool m_isPlayerDead = false;

	Vector2 m_originPos;
	float m_originAngle;

	bool m_toDoMove = false;
	Vector2 m_movingDestPos;
	Vector2 m_moveDirection;

	float m_rotationSpeed = 300.0f;
	float m_acceleration = 0.0f;

	float m_moveSpeed = 200.0f;
	float m_maxSightDistance = 600.0f;
	float m_sightAngle = 90.0f;
	float m_maxRoamDistance = 1000.0f;
	float m_maxChaseDistance = 1200.0f;
	float m_AttackRange = 20.0f;

	bool m_isLockOnTarget = false;
	bool m_isTargetInChaseDist = false;
	bool m_isTargetInRoamDist = false;
	bool m_isTargetInAtkRange = false;

	float m_atkAngle = 10.0f;
	bool m_isTargetInAtkAngle = false;

	float m_attackInterval = 1.5f;
	float m_attackTimer = 0.0f;
	bool m_isAttackReady = true;

	float m_AttackEffectTime = 0.4f;
	float m_atkColTimer = 0.0f;

	bool m_isUnderAttack = false;
	float m_underAttackValidTime = 0.2f;
	float m_underAttackTimer = 0.0f;

	bool m_isFindPlayer = false;



	void CheckCameraArea();

	void CheckAndTransitState();

	void SetTargetPlayerAndCamera(GameObject* player, GameObject* camera);



	void PositionInit(float x, float y, float angle);

	void MoveTo(const Vector2& destination);

	void StopMoving();

	void SetAnimDirection();

	void UpdateDirection();

	void UpdateMovement();

	void RigidBodyUpdate();

	void SetRotationAngle(float angle);

	void SetAngleByDirection(int n);

	void FakeAttack();

	void AtkColliderLifeManage();

	void AttackCoolCheck();

	void UnderAttackManage();



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

	inline bool& IsDead() { return m_isDead; }

	inline int& CurrEnemyState() { return m_currEnemyState; }

	inline int& PrevEnemyState() { return m_prevEnemyState; }

	inline float& RotationAngle() { return m_rotationAngle; }

	inline int& PrevDir() { return m_prevDir; }

	inline int& CurrDir() { return m_currDir; }

	inline float& OriginAngle() { return m_originAngle; }

	inline float& MoveSpeed() { return m_moveSpeed; }

	inline float& MaxSightDistance() { return m_maxSightDistance; }

	inline float& SightAngle() { return m_sightAngle; }

	inline float& MaxRoamDistance() { return m_maxRoamDistance; }

	inline float& MaxChaseDistance() { return m_maxChaseDistance; }

	inline float& AttackRange() { return m_AttackRange; }

	inline int& Direction() { return m_direction; }

	inline const Vector2& CameraPos() { return m_camera->GetTransform()->GetLocalPosition(); }

	inline bool& IsInCamera() { return m_isInCamera; }

	inline const Vector2& TargetPos() { return m_player->GetTransform()->GetLocalPosition(); }

	inline Vector2& OriginPos() { return m_originPos; }

	inline Vector2& MovingDestPos() { return m_movingDestPos; }

	inline bool& ToDoMove() { return m_toDoMove; }

	inline bool& IsLockOnTarget() { return m_isLockOnTarget; }

	inline bool& IsTargetInChaseDist() { return m_isTargetInRoamDist; }

	inline bool& IsTargetInRoamDist() { return m_isTargetInChaseDist; }

	inline bool& IsTargetInAtkRange() { return m_isTargetInAtkRange; }

	inline float& AtkAngle() { return m_atkAngle; }

	inline bool& IsTargetInAtkAngle() { return m_isTargetInAtkAngle; }

	inline bool& IsAttackReady() { return m_isAttackReady; }

	inline bool& IsUnderAttack() { return m_isUnderAttack; }

	inline bool& IsFindPlayer() { return m_isFindPlayer; }

	inline const bool& IsPlayerDead()
	{
		m_context.boolParams[L"IsPlayerDead"] = m_isPlayerDead;
		return m_context.boolParams[L"IsPlayerDead"];
	}

	inline const bool IsPlayerNull()
	{
		if (m_player == nullptr)
			return true;

		return false;
	}

	RigidBody2D* RigidBody();

	const Vector2& Pos();

};

