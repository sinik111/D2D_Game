#pragma once

#include "Component.h"
#include "Delegate.h"

class BitmapRenderer;

class Button :
	public Component
{
private:
	Delegate<> m_onClickButton;
	BitmapRenderer* m_bitmapRenderer = nullptr;

public:
	void RegisterToSystem() override;
	void UnregisterFromSystem() override;

	void Initialize() override;

public:
	template<typename T>
	void AddListener(T* instance, void (T::* func)())
	{
		m_onClickButton.Add(instance, func);
	}

	bool IsContainPoint(const Vector2& point);
};