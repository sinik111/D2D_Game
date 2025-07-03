#pragma once

#include <unordered_map>
#include <string>

class Transform;
class Animator;

struct FSMContext
{
	Transform* transform;
	Animator* animator;
	std::unordered_map<std::wstring, int>* intParams;
	std::unordered_map<std::wstring, float>* floatParams;
	std::unordered_map<std::wstring, bool>* boolParams;
	std::unordered_map<std::wstring, bool>* triggerParams;
	std::wstring nextStateName;
	bool hasRequest;
};