#pragma once

#include <functional>

#include "Component.h"
#include "Vector2.h"

class PlayerInput :
	public Component
{
public:
	enum class DirectionInputType
	{
		WASD,
		Arrow,
		Both
	};

	enum class InputCheckType
	{
		Released,
		Pressed,
		Down
	};

private:
	std::vector<std::function<void(Vector2)>> m_directionActions;
	std::vector<std::vector<std::function<void()>>> m_actionsOnKeyReleased;
	std::vector<std::vector<std::function<void()>>> m_actionsOnKeyPressed;
	std::vector<std::vector<std::function<void()>>> m_actionsOnKeyDown;

	DirectionInputType m_directionInputType;
	Vector2 m_mousePosition;

public:
	PlayerInput();
	~PlayerInput() override;

public:
	void RegisterActionOnKey(short vKey, InputCheckType checkType, std::function<void()> func);
	void UnregisterActionsOnKey(short vKey, InputCheckType checkType);

	void RegisterDirectionAction(DirectionInputType type, std::function<void(Vector2)> func);
	void UnregisterDirectionActions();

	Vector2 GetMousePosition() const;

public:
	template<typename T>
	static std::function<void(Vector2)> MakeDirectionAction(void (T::* func)(Vector2), T* comp)
	{
		static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

		return std::bind(func, comp, std::placeholders::_1);
	}

	template<typename T>
	static std::function<void()> MakeAction(void (T::* func)(), T* comp)
	{
		static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

		return std::bind(func, comp);
	}

private:
	void CallWASDAction(float horizontal, float vertical);
	void CallArrowAction(float horizontal, float vertical);
	void CallActionOnReleased(short vKey);
	void CallActionOnPressed(short vKey);
	void CallActionOnDown(short vKey);
	void SetMousePosition(float x, float y);

	friend class PlayerInputSystem;
};