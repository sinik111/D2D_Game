#pragma once

#include <functional>

template<typename... Args>
class Action
{
private:
	std::function<void(Args...)> m_func = nullptr;

public:
	template<typename Func>
	Action(Func&& func)
		: m_func{ std::forward<Func>(func) }
	{

	}

	template<typename T>
	Action(T* instance, void (T::*func)(Args...), bool safeAction = true)
	{
		if (safeAction)
		{
			m_func = [instance, func](Args... args) {
				if (Object::IsValid(instance))
				{
					(instance->*func)(args...);
				}
			};
		}
		else
		{
			m_func = [instance, func](Args... args) {
				(instance->*func)(args...);
			};
		}
	}

	void operator()(Args... args) const
	{
		if (m_func)
		{
			m_func(std::forward<Args>(args)...);
		}
	}

	explicit operator bool() const
	{
		return static_cast<bool>(m_func);
	}
};