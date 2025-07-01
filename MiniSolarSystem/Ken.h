#pragma once

#include "../D2DEngineLib/Script.h"

class Animator;

enum class KenState
{
	IDLE,
	FRONT_DASH,
	BACK_DASH,
	ROLL,
	SPINNING_KICK
};

class Ken :
	public Script
{
private:
	Animator* m_animator;
	float m_speed = 0.0f;
	int m_moveDir = 0;

private:
	void Initialize() override;
	void Start() override;
	void Update() override;

private:
	void ChangeState(KenState state);
	void Idle();
	void FrontDash();
	void BackDash();
	void Roll();
	void SpinningKick();
	void ChangeScene();
};