#pragma once

#include "../D2DEngineLib/Script.h"
#include "KenFSM.h"

class Animator;

class Ken :
	public Script
{
private:
	std::unique_ptr<KenFSM> m_kenFSM;
	Animator* m_animator;

	std::unordered_map<std::wstring, float> m_floatParams;
	std::unordered_map<std::wstring, bool> m_triggerParams;

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