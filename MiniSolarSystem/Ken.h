#pragma once

#include "../D2DEngineLib/Script.h"

class Animator;

class Ken :
	public Script
{
private:
	Animator* m_animator;
	int m_moveDir = 0;

private:
	void Initialize() override;
	void Start() override;
	void Update() override;

private:
	void Idle();
	void FrontDash();
	void BackDash();
	void Roll();
	void SpinningKick();
	void ChangeScene();
	void FireEarth();
};