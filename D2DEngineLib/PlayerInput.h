#pragma once

#include <functional>

#include "Component.h"
#include "Vector2.h"
#include "Action.h"
#include "Delegate.h"

enum class DirectionInputType
{
	WASD,
	Arrow,
	Both
};

enum class KeyState
{
	Released,
	Pressed,
	Held
};

class PlayerInput :
	public Component
{
private:
	struct SingleKeyActionData
	{
		short vKey;
		KeyState checkType;
		void* instance;
		Action<> action;
	};

	struct CombinedKeyActionData
	{
		std::vector<short> vKeys;
		KeyState checkType; // 마지막 키
		void* instance;
		Action<> action;
	};

private:
	Delegate<Vector2> m_directionDelegateWASD;
	Delegate<Vector2> m_directionDelegateArrow;
	std::vector<SingleKeyActionData> m_singleKeyActions;
	std::vector<CombinedKeyActionData> m_combinedKeyActions;

	Vector2 m_mousePosition;
	DirectionInputType m_directionInputType = DirectionInputType::Both;

public:
	PlayerInput();
	~PlayerInput() override;

public:
	template<typename T>
	void RegisterActionOnKey(short vKey, KeyState checkType, T* instance, void (T::* func)())
	{
		m_singleKeyActions.push_back({ vKey, checkType, instance, Action<>(instance, func) });
	}

	template<typename T>
	void RegisterActionOnCombinedKey(const std::vector<short>& vKeys, KeyState checkType, T* instance, void (T::* func)())
	{
		m_combinedKeyActions.push_back({ vKeys, checkType, instance, Action<>(instance, func) });
	}

	template<typename T>
	void RegisterDirectionAction(DirectionInputType type, T* instance, void (T::* func)(Vector2))
	{
		m_directionInputType = type;

		if (type == DirectionInputType::WASD || type == DirectionInputType::Both)
		{
			m_directionDelegateWASD.Add(instance, func);
		}

		if (type == DirectionInputType::Arrow || type == DirectionInputType::Both)
		{
			m_directionDelegateArrow.Add(instance, func);
		}
	}

	void UnregisterActionOnKey(short vKey, void* instance);
	void UnregisterActionOnKey(short vKey, KeyState checkType, void* instance);
	void UnregisterCombinedAction(const std::vector<short>& vKeys, void* instance);
	void UnregisterCombinedAction(const std::vector<short>& vKeys, KeyState checkType, void* instance);
	void UnregisterDirectionActions(void* instance);

	Vector2 GetMousePosition() const;

private:
	void CallActions();
	void CallArrowAction(float horizontal, float vertical);
	void CallWASDAction(float horizontal, float vertical);
	void SetMousePosition(float x, float y);

	friend class PlayerInputSystem;
};