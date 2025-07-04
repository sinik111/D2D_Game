#pragma once

#include <unordered_map>
#include <string>

class Transform;
class Animator;
class BitmapRenderer;

struct FSMContext
{
	Transform* transform;
	Animator* animator;
	BitmapRenderer* bitmapRenderer;
	std::unordered_map<std::wstring, int> intParams;
	std::unordered_map<std::wstring, float> floatParams;
	std::unordered_map<std::wstring, bool> boolParams;
	std::unordered_map<std::wstring, bool> triggerParams;
	std::wstring currentStateName;
	std::wstring nextStateName;
	bool shouldChangeState;

	void Reset()
	{
		shouldChangeState = false;
		nextStateName = L"";

		for (auto& pair : triggerParams)
		{
			pair.second = false;
		}
	}
};