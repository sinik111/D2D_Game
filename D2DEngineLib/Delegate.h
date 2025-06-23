#pragma once

#include "Action.h"

template<typename... Args>
class Delegate
{
private:
	struct CallbackInfo
	{
		void* instance;
		Action<Args...> action;
	};

private:
	std::vector<CallbackInfo> m_callbackInfos;

public:
	template<typename T>
	void Add(T* instance, void (T::*func)(Args...))
	{
		m_callbackInfos.push_back({ instance, Action<Args...>(instance, func)});
	}

	void Remove(void* instance)
	{
		m_callbackInfos.erase(
			std::remove_if(
				m_callbackInfos.begin(),
				m_callbackInfos.end(),
				[instance](const CallbackInfo& info) {
					return info.instance == instance;
				}
			),
			m_callbackInfos.end()
		);
	}

	void Clear()
	{
		m_callbackInfos.clear();
	}

	void Invoke(Args... args) const
	{
		for (const auto& info : m_callbackInfos)
		{
			if (info.action)
			{
				info.action(args...);
			}
		}
	}
};