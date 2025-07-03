#pragma once

#include "../D2DEngineLib/Script.h"
#include "KenFSM.h"
#include "FSMContext.h"

class Animator;

class Ken :
	public Script
{
private:
	Animator* m_animator;
	std::unique_ptr<KenFSM> m_kenFSM;

	FSMContext m_context;

private:
	void Initialize() override;
	void Start() override;
	void Update() override;

private:
	void ArrowInput(Vector2 input);
	void Roll();
	void SpinningKick();
	void ChangeScene();
	void FireEarth();
};