#pragma once

#include <sstream>

#include "../D2DEngineLib/Script.h"
#include "../D2DEngineLib/FSM.h"
#include "../D2DEngineLib/FSMContext.h"

class RigidBody2D;

enum class PlayerDirection
{
	LeftUp,
	Up,
	RightUp,
	Right,
	RightDown,
	Down,
	LeftDown,
	Left,
	Max
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
	void HeavyAttackKeyPressed();
	void HeavyAttackKeyHeld();
	void HeavyAttackKeyReleased();

	void UpdateTimers();

public:
	const PlayerStat& GetPlayerStat() const;
	PlayerStatus& GetPlayerStatus();

public:
	void SetPlayerInfoTextRenderer(TextRenderer* textRenderer);

public:
	static Vector2 CalculateDirectionVector(PlayerDirection direction);
	static Vector2 CalculateDirectionVector(int direction);
};

inline std::wostringstream& operator<<(std::wostringstream& woss, const PlayerStat& playerStat);
inline std::wostringstream& operator<<(std::wostringstream& woss, const PlayerStatus& playerStatus);