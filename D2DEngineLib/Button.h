#pragma once

#include "Component.h"
#include "Delegate.h"
#include "Enum.h"

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
	void AddAction(T* instance, void (T::* func)())
	{
		m_onClickButton.Add(instance, func);
	}

	bool IsContainPoint(const Vector2& point);
	SpaceType GetSpaceType() const;
	void InvokeAction();
	void SetClickedColor(const D2D1_COLOR_F& color);
	void ResetColor();
};