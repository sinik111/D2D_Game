#pragma once

#include "Object.h"
#include "Action.h"

template<typename... Args>
class Delegate
{
private:
	struct CallbackInfo
	{
		Object* instance;
		Action<Args...> action;
	};

private:
	std::vector<CallbackInfo> m_callbackInfos;

public:
	template<typename T>
	void Add(T* instance, void (T::*func)(Args...))
	{
		m_callbackInfos.push_back({ instance, Action<Args...>(instance, func, false) });
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

	void Invoke(Args... args)
	{
		for (auto iter = m_callbackInfos.begin(); iter != m_callbackInfos.end();)
		{
			if (!Object::IsValid(iter->instance))
			{
				iter = m_callbackInfos.erase(iter);

				continue;
			}

			iter->action(args...);

			++iter;
		}
	}
};