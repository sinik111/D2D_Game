#pragma once

#include "../D2DEngineLib/Script.h"
#include "KenFSM.h"
#include "FSMContext.h"

class Animator;
class Health;

class Ken :
	public Script
{
private:
	Animator* m_animator;
	Health* m_health;
	std::unique_ptr<KenFSM> m_kenFSM;

	FSMContext m_context;

	bool m_isJump = false;

private:
	void Initialize() override;
	void Start() override;
	void FixedUpdate() override;
	void Update() override;

private:
	void ArrowInput(Vector2 input);
	void Roll();
	void SpinningKick();
	void ChangeScene();
	void FireEarth();
	void TakeDamage();
	void Revive();
};