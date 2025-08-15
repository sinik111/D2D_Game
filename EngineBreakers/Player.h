#pragma once

#include <sstream>
#include <array>

#include "../D2DEngineLib/Script.h"
#include "../D2DEngineLib/FSM.h"
#include "../D2DEngineLib/FSMContext.h"

using AnimPathArray = std::array<std::wstring, 10>;

class RigidBody2D;
class BoxCollider2D;
class CircleCollider;
class EnemyBase;

enum class PlayerDirection
{
	NoDirection = 0, //0
	LeftDown = 1,	 //1
	Down = 2,		 //2
	RightDown = 3,	 //3
	Left = 4,		 //4
	Max = 5,		 //5
	Right = 6,		 //6
	LeftUp = 7,		 //7
	Up = 8,			 //8
	RightUp = 9		 //9
};

struct PlayerStat
{
	int maxHp = 40;
	float maxStamina = 50.0f;
	float staminaRestoreAmountPerSecond = 16.7f;
	int attackPowerMin = 9;
	int attackPowerMax = 11;
	int heavyAttackPowerMin = 11;
	int heavyAttackPowerMax = 13;
	int grabAttackPowerMin = 13;
	int grabAttackPowerMax = 15;
	float attackStaminaCost = 11.9f;
	float heavyAttackStaminaCost = 20.0f;
	int attackKnockdownPower = 3;
	int heavyAttackKnockdownPower = 4;
	float attackInterval = 0.4f;
	int knockbackResist = 6;
	int knockdownResist = 10;
	float knockdownResetTime = 5.0f;
	float moveSpeed = 200.0f;
	float dashSpeed = 400.0f;
	float dashStaminaPerSec = 9.0f;
	float evadeDistance = 300.0f;
	float evadeDuration = 0.3f;
	float evadeAvailableTime = 0.2f;
	float evadeInterval = 1.5f;
	float evadeStamina = 14.0f;
};

struct PlayerStatus
{
	int currentHp = 40;
	float currentStamina = 50.0f;
	float attackIntervalTimer = 0.0f;
	int currentKnockdownAmount = 0;
	float knockdownResetTimer = 0.0f;
	float evadeDurationTimer = 0.0f;
	float evadeAvailalbeTimer = 0.0f;
	float evadeIntervalTimer = 0.0f;
};

class Player :
	public Script
{
private:
	RigidBody2D* m_rigidBody2d = nullptr;
	TextRenderer* m_playerInfoTextRenderer = nullptr;

	FSM m_playerFSM;
	FSMContext m_fsmContext;

	PlayerStat m_playerStat;
	PlayerStatus m_playerStatus;

	Animator* m_animator;
	BoxCollider2D* m_boxCollider;
	CircleCollider* m_CircleCollider;

private:
	void Initialize() override;
	void Start() override;
	void FixedUpdate() override;
	void Update() override;

	void OnTriggerStay(const Collision& collision) override;

private:
	void SetDirectionInput(Vector2 input);
	void DashKeyPressed();
	void DashKeyReleased();
	void NormalAttackKeyPressed();
	void NormalAttackKeyReleased();
	void HeavyAttackKeyPressed();
	void HeavyAttackKeyHeld();
	void HeavyAttackKeyReleased();



	void UpdateTimers();

	void NormalAttackAllKeyPressed();

public:
	const PlayerStat& GetPlayerStat() const;
	PlayerStatus& GetPlayerStatus();

public:
	void SetPlayerInfoTextRenderer(TextRenderer* textRenderer);

public:
	static Vector2 CalculateDirectionVector(PlayerDirection direction);
	static Vector2 CalculateDirectionVector(int direction);

	void SetPlayerNextState(const std::wstring& nextState);

//====

public:
	void ChangeStateAndAnim(const std::wstring& stateName);
	void ChangeStateWalkOrIdle();
	
public:

	std::wstring m_currentStateName;

	Vector2 m_aheadDirctionVectorMove = Vector2::Zero;
	float m_currPlayerSpeed = 0.0f;

	Vector2 m_attackDirection = Vector2::Zero;
	Vector2 m_attackStartPos = Vector2::Zero;
	float m_attackMoveDistSq = 0.0f;

	float m_acceleration = 0.0f;
	float m_drag = 0.0f;
	int m_movingAheadDirection = 5;
	bool m_onHoverMode = false;
	float m_debugLogTimer = 0.0f;	

	bool m_onAttack = false;

	bool m_onForceMove = false;

	bool m_fullSpeedWalk = false;
	bool m_isGameOver = false;

	void SetIsGameOver(bool over);

	float m_evadeOnlyVelocity = 1560.0f;
	

	void LoadJson(std::wstring path);

	inline int PDir(const PlayerDirection& dir) { return static_cast<int>(dir); }

	int m_animDirection = PDir(PlayerDirection::Down);
	int m_prevDir = PDir(PlayerDirection::Down);
	int m_currDir = PDir(PlayerDirection::Down);
	
	void AnimationInit();
	void InitAnimPath(AnimPathArray& animPath, const std::wstring& animName);
	void SetAnimState(int animState);

	

	void UpdateAnimDirection();

	void UpdateRigidbody();

	void UpdateAttackColliderPosition();

	void CheckGrabAttack();

	void RegenerateStamina();

	std::wstring m_currStateName = L"";
	std::wstring m_nextStateName = L"";

	inline std::wstring& GetCurrStateName() { return m_currStateName; }
	inline std::wstring& GetNextStateName() { return m_nextStateName; }

	AnimPathArray m_idleAnimPath;		
	
	AnimPathArray m_dashIngAnimPath;
	AnimPathArray m_dashInAnimPath;
	AnimPathArray m_dashEndAnimPath;
	
	AnimPathArray m_knockdownAnimPath;
		
	AnimPathArray m_normalAttackOnceAnimPath;
	AnimPathArray m_normalAttackTwiceAnimPath;
	AnimPathArray m_normalAttackThreeTimesAnimPath;
	
	AnimPathArray m_heavyAttackAnimPath;
	AnimPathArray m_heavyAttackTwiceAnimPath;
	
	AnimPathArray m_normalAttackAnimPath;
	AnimPathArray m_normalAttackAllAnimPath;
	
	AnimPathArray m_grabAttackAnimPath;
	
	AnimPathArray m_walkInAnimPath;
	AnimPathArray m_walkIngAnimPath;
	AnimPathArray m_walkEndAnimPath;
	
	AnimPathArray m_jumpUpAnimPath;
	AnimPathArray m_jumpDownAnimPath;



	AnimPathArray m_animPath = {
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

	AnimPathArray m_animPathdir = {
		L"S",     // 0 
		L"SW",    // 1
		L"S",     // 2 
		L"SE",    // 3
		L"W",     // 4
		L"S",     // 5 
		L"E",     // 6
		L"NW",    // 7
		L"N",     // 8
		L"NE"     // 9
	};

	enum PlayerAnimState
	{
		NoAnimation,
		PlayerAnimIdle,
		PlayerAnimWalkIn,
		PlayerAnimWalkIng,
		PlayerAnimWalkEnd,
		PlayerAnimDashIn,
		PlayerAnimDashIng,
		PlayerAnimDashEnd,
		PlayerAnimNormalAttackOnce,
		PlayerAnimNormalAttackTwice,
		PlayerAnimNormalAttackThreeTimes,		
		PlayerAnimHeavyAttack,
		PlayerAnimHeavyAttackTwice,
		PlayerAnimNormalAttack,
		PlayerAnimNormalAttackAll,		
		PlayerAnimGrabAttack,
		PlayerAnimJumpUp,
		PlayerAnimJumpDown,		
		PlayerAnimKnockdown,
	
	};

public:
	int ConvertVector2ToKeypadDirection(const Vector2& vec);
	

	bool m_isGetDmg = false;

	int m_gettingDmg = 0;
	int m_gettingKnock = 0;
	Vector2 m_KnockDir = Vector2::Zero;


	void AddDmg(int d)
	{
		m_gettingDmg += d;
	}

	void AddKnock(int pd, Vector2 pv);

	void UpdateDmg();

	float m_kTimer = 0.0f;

	bool m_isKnockback = false;
	

	bool m_isDead = false;

	bool GetPlayerIsDead() { return m_isDead; }

	GameObject* m_normalAttackGo = nullptr;
	GameObject* m_heavyAttackGo = nullptr;
	GameObject* m_checkKnockdownkGo = nullptr;

	void UpdateAttackCombo();
	bool m_isAttackPressed = false;	
	bool m_attackPressedCheck = false;	

	bool m_isComboInputAvailable = false;  
	bool m_isNextAttackQueued = false;     
	float m_comboWaitTimer = 0.0f; 
	float m_comboWaitDuration = 0.6f;
	bool m_isComboWait = false;
	std::wstring m_lastAttackName;

	bool m_isOnHeavyAttackOnce = false;
	bool m_isReadyHeavyAttackTwice = false;

	
	bool m_isCanAttack = true;	

	bool m_isReadyGrabAttack = false;
	EnemyBase* m_grabAttackTarget = nullptr;
	Vector2 m_grabMoveToPosition;	

	void PlayerNormalAttackOnce(float duration);
	void PlayerNormalAttackTwice(float duration);
	void PlayerNormalAttackThreeTimes(float duration);	

	void PlayerHeavyAttack1(float duration);

	void HitStopUpdate();
	void ForceHitStop(float scale, float duration);

	bool m_attackHit = false;
	bool m_isOnForceHitStop = false;
	float m_hitStopDuration = 0.1f;
	float m_hitStopTimer = 0.0f;
	float m_forceHitStopDuration = 0.0f;


	Vector2 m_normalAttackColliderPos = Vector2::Zero;
	Vector2 m_heavyAttackColliderPos = Vector2::Zero;
	
	float m_grabDistance = 200.0f;
	
	float m_attackNormalOnceDuration = 0.67f;
	float m_attackNormalTwiceDuration = 0.51f;
	float m_attackNormalThreeTimesDuration = 0.54f;
	
	float m_attackHeavyDuration = 1.0f;
	
	float m_attackNormalOnceMoveSpd = 900.0f;
	float m_attackNormalTwiceMoveSpd = 600.0f;
	float m_attackNormalThreeTimesMoveSpd = 400.0f;

	float m_attackHeavyOnceMoveSpd = 900.0f;
	float m_attackHeavyTwiceMoveSpd = 900.0f;
	
	float m_attackC_1MoveSpd = 900.0f;

	float m_attack1_1SafetyTime = 0.2f;
	float m_attack1_2SafetyTime = 0.1f;
	float m_attack1_3SafetyTime = 0.1f;

	float m_attack1_1EndTime = 0.48f;
	float m_attack1_2EndTime = 0.35f;
	float m_attack1_3EndTime = 0.45f;

public:
	//Jump
	bool m_isJump = false;
	bool m_isWalkToJump = false;
	bool m_isJumptoUP = false;
	bool m_isColliding = false;
	Vector2 m_landPos;
	Vector2 m_JumpStartDirection;
	int m_JumpStartDirectionIndex=0;

	void Jump();
	void JumpUp();
	void JumpDown();
	void WalktoJump();

// Animation Events
public:
	void Footstep1();
	void Footstep2();
	void AttackTry1();
	void AttackTry23();
	void AttackSuccess();
	void Damaged();
	void DashIn();
	void Dashing();
	void DashEnd();
	void JumpEvent();
};

inline std::wostringstream& operator<<(std::wostringstream& woss, const PlayerStat& playerStat);
inline std::wostringstream& operator<<(std::wostringstream& woss, const PlayerStatus& playerStatus);