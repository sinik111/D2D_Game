#pragma once

#include <functional>

template<typename... Args>
class Action
{
private:
	std::function<void(Args...)> m_func;

public:
	Action() = default;

	template<typename Func>
	Action(Func&& func)
		: m_func(std::forward<Func>(func))
	{

	}

	template<typename T>
	Action(T* instance, void (T::*func)(Args...))
	{
		m_func = [instance, func](Args... args)
			{
				(instance->*func)(std::forward<Args>(args)...);
			};
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