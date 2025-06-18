#include "pch.h"
#include "ScriptSystem.h"

#include "ContainerUtility.h"
#include "Script.h"

void ScriptSystem::Register(Script* script)
{
	m_scripts.push_back(script);
	m_scriptsForPendingStart.push_back(script);
}

void ScriptSystem::Unregister(Script* script)
{
	Util::OptimizedErase(m_scripts, script);
	Util::OptimizedErase(m_scriptsForPendingStart, script);
	Util::OptimizedErase(m_scriptsForStart, script);
	Util::OptimizedErase(m_scriptsForUpdate, script);
	Util::OptimizedErase(m_scriptsForLateUpdate, script);
}

void ScriptSystem::UnregisterUpdate(Script* script)
{
	m_pendingUnregisterForUpdate.push_back(script);
}

void ScriptSystem::UnregisterLateUpdate(Script* script)
{
	m_pendingUnregisterForLateUpdate.push_back(script);
}

void ScriptSystem::UpdateSystem()
{
	CallStart();
	CallUpdate();
	CallLateUpdate();
}

void ScriptSystem::CallStart()
{
	while (!m_scriptsForPendingStart.empty())
	{
		m_scriptsForStart.insert(m_scriptsForStart.end(),
			std::make_move_iterator(m_scriptsForPendingStart.begin()),
			std::make_move_iterator(m_scriptsForPendingStart.end()));

		m_scriptsForPendingStart.clear();


		if (!m_scriptsForStart.empty())
		{
			for (auto script : m_scriptsForStart)
			{
				script->Start();

				m_scriptsForUpdate.push_back(script);
				m_scriptsForLateUpdate.push_back(script);
			}

			m_scriptsForStart.clear();
		}
	}
}

void ScriptSystem::CallUpdate()
{
	for (auto script : m_scriptsForUpdate)
	{
		script->Update();
	}

	if (!m_pendingUnregisterForUpdate.empty())
	{
		for (auto script : m_pendingUnregisterForUpdate)
		{
			Util::OptimizedErase(m_scriptsForUpdate, script);
		}

		m_pendingUnregisterForUpdate.clear();
	}
}

void ScriptSystem::CallLateUpdate()
{
	for (auto script : m_scriptsForLateUpdate)
	{
		script->LateUpdate();
	}

	if (!m_pendingUnregisterForLateUpdate.empty())
	{
		for (auto script : m_pendingUnregisterForLateUpdate)
		{
			Util::OptimizedErase(m_scriptsForLateUpdate, script);
		}

		m_pendingUnregisterForLateUpdate.clear();
	}
}