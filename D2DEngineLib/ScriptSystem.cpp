#include "pch.h"
#include "ScriptSystem.h"

#include "ContainerUtility.h"
#include "Script.h"

void ScriptSystem::Register(Script* script)
{
	if (script->m_isStarted)
	{
		m_scriptsForFixedUpdate.push_back(script);
		m_scriptsForUpdate.push_back(script);
		m_scriptsForLateUpdate.push_back(script);
	}
	else
	{
		m_scriptsForStart.push_back(script);
	}
}

void ScriptSystem::Unregister(Script* script)
{
	m_pendingUnregisteredScripts.push_back(script);
}

void ScriptSystem::UnregisterFixedUpdate(Script* script)
{
	m_pendingUnregisteredFixedUpdate.push_back(script);
}

void ScriptSystem::UnregisterUpdate(Script* script)
{
	m_pendingUnregisteredUpdate.push_back(script);
}

void ScriptSystem::UnregisterLateUpdate(Script* script)
{
	m_pendingUnregisteredLateUpdate.push_back(script);
}

void ScriptSystem::CallStart()
{
	for (size_t i = 0; i < m_scriptsForStart.size(); ++i)
	{
		auto& script = m_scriptsForStart[i];

		script->Start();
		script->m_isStarted = true;

		m_scriptsForFixedUpdate.push_back(script);
		m_scriptsForUpdate.push_back(script);
		m_scriptsForLateUpdate.push_back(script);
	}

	m_scriptsForStart.clear();
}

void ScriptSystem::CallFixedUpdate()
{
	UpdateScriptsList();

	for (size_t i = 0; i < m_scriptsForFixedUpdate.size(); ++i)
	{
		m_scriptsForFixedUpdate[i]->FixedUpdate();
	}

	UpdateScriptsList();
}

void ScriptSystem::CallUpdate()
{
	UpdateScriptsList();

	for (size_t i = 0; i < m_scriptsForUpdate.size(); ++i)
	{
		m_scriptsForUpdate[i]->Update();
	}

	UpdateScriptsList();
}

void ScriptSystem::CallLateUpdate()
{
	UpdateScriptsList();

	for (size_t i = 0; i < m_scriptsForLateUpdate.size(); ++i)
	{
		m_scriptsForLateUpdate[i]->LateUpdate();
	}

	UpdateScriptsList();
}

void ScriptSystem::UnregisterScripts()
{
	std::sort(m_pendingUnregisteredScripts.begin(), m_pendingUnregisteredScripts.end());

	Util::OptimizedErase(m_scriptsForFixedUpdate, m_pendingUnregisteredScripts, true);
	Util::OptimizedErase(m_scriptsForUpdate, m_pendingUnregisteredScripts, true);
	Util::OptimizedErase(m_scriptsForLateUpdate, m_pendingUnregisteredScripts, true);

	m_pendingUnregisteredScripts.clear();
}

void ScriptSystem::UpdateScriptsList()
{
	if (!m_pendingUnregisteredScripts.empty())
	{
		UnregisterScripts();
	}

	if (!m_scriptsForStart.empty())
	{
		CallStart();
	}
}