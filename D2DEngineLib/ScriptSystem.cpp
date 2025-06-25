#include "pch.h"
#include "ScriptSystem.h"

#include "ContainerUtility.h"
#include "Script.h"

void ScriptSystem::Register(Script* script)
{
	m_scripts.push_back(script);
	m_scriptsForPendingInitialize.push_back(script);
}

void ScriptSystem::Unregister(Script* script)
{
	Util::OptimizedErase(m_scripts, script);
	Util::OptimizedErase(m_scriptsForPendingInitialize, script);
	Util::OptimizedErase(m_scriptsForInitialize, script);
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
	CallInitialize();
	CallStart();
	CallUpdate();
	CallLateUpdate();
}

void ScriptSystem::CallInitialize()
{
	while (!m_scriptsForPendingInitialize.empty())
	{
		m_scriptsForInitialize.insert(m_scriptsForInitialize.end(),
			std::make_move_iterator(m_scriptsForPendingInitialize.begin()),
			std::make_move_iterator(m_scriptsForPendingInitialize.end()));

		m_scriptsForPendingInitialize.clear();


		if (!m_scriptsForInitialize.empty())
		{
			for (const auto& script : m_scriptsForInitialize)
			{
				script->Initialize();

				m_scriptsForStart.push_back(script);
			}

			m_scriptsForInitialize.clear();
		}
	}
}

void ScriptSystem::CallStart()
{
	if (!m_scriptsForStart.empty())
	{
		for (const auto& script : m_scriptsForStart)
		{
			script->Start();

			m_scriptsForUpdate.push_back(script);
			m_scriptsForLateUpdate.push_back(script);
		}

		m_scriptsForStart.clear();
	}
}

void ScriptSystem::CallUpdate()
{
	for (const auto& script : m_scriptsForUpdate)
	{
		script->Update();
	}

	if (!m_pendingUnregisterForUpdate.empty())
	{
		for (const auto& script : m_pendingUnregisterForUpdate)
		{
			Util::OptimizedErase(m_scriptsForUpdate, script);
		}

		m_pendingUnregisterForUpdate.clear();
	}
}

void ScriptSystem::CallLateUpdate()
{
	for (const auto& script : m_scriptsForLateUpdate)
	{
		script->LateUpdate();
	}

	if (!m_pendingUnregisterForLateUpdate.empty())
	{
		for (const auto& script : m_pendingUnregisterForLateUpdate)
		{
			Util::OptimizedErase(m_scriptsForLateUpdate, script);
		}

		m_pendingUnregisterForLateUpdate.clear();
	}
}