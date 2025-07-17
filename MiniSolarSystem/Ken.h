#pragma once

#include "../D2DEngineLib/Script.h"
#include "../D2DEngineLib/FSM.h"
#include "../D2DEngineLib/FSMContext.h"

class Animator;
class Health;
class RigidBody2D;

class Ken :
	public Script
{
private:
	Animator* m_animator;
	Health* m_health;
	RigidBody2D* m_rigidBody;

	FSM m_fsm;
	FSMContext m_context;

	bool m_isJump = false;
	bool m_isGround = false;

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
	void Roll();
	void SpinningKick();
	void ChangeScene();
	void FireEarth();
	void TakeDamage();
	void Revive();
};