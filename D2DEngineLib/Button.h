#pragma once

#include "Component.h"
#include "Delegate.h"

class Button :
	public Component
{
private:
	Delegate<> m_onClickButton;
	Bounds m_bounds;

public:
	void RegisterToSystem() override;
	void UnregisterFromSystem() override;

public:
	template<typename T>
	void AddListener(T* instance, void (T::* func)())
	{
		m_onClickButton.Add(instance, func);
	}

	//bool IsContainPoint(const Vector2& point);
};