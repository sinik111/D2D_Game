#include "pch.h"
#include "ScriptSystem.h"

#include "ContainerUtility.h"
#include "Script.h"

void ScriptSystem::Register(Script* script)
{
	m_scriptsForStart.push_back(script);
}

void ScriptSystem::Unregister(Script* script)
{
	Util::OptimizedErase(m_scriptsForInitialize, script);
	Util::OptimizedErase(m_scriptsForStart, script);
	Util::OptimizedErase(m_scriptsForFixedUpdate, script);
	Util::OptimizedErase(m_scriptsForUpdate, script);
	Util::OptimizedErase(m_scriptsForLateUpdate, script);
	Util::OptimizedErase(m_scriptsForOnRender, script);
}

void ScriptSystem::UnregisterFixedUpdate(Script* script)
{
	m_pendingUnregisterForFixedUpdate.push_back(script);
}

void ScriptSystem::UnregisterUpdate(Script* script)
{
	m_pendingUnregisterForUpdate.push_back(script);
}

void ScriptSystem::UnregisterLateUpdate(Script* script)
{
	m_pendingUnregisterForLateUpdate.push_back(script);
}

void ScriptSystem::UnregisterOnRender(Script* script)
{
	m_pendingUnregisterForOnRender.push_back(script);
}

void ScriptSystem::CallStart()
{
	if (!m_scriptsForStart.empty())
	{
		for (const auto& script : m_scriptsForStart)
		{
			script->Start();

			m_scriptsForFixedUpdate.push_back(script);
			m_scriptsForUpdate.push_back(script);
			m_scriptsForLateUpdate.push_back(script);
			m_scriptsForOnRender.push_back(script);
		}

		m_scriptsForStart.clear();
	}
}

void ScriptSystem::CallFixedUpdate()
{
	for (const auto& script : m_scriptsForFixedUpdate)
	{
		script->FixedUpdate();
	}

	if (!m_pendingUnregisterForFixedUpdate.empty())
	{
		for (const auto& script : m_pendingUnregisterForFixedUpdate)
		{
			Util::OptimizedErase(m_scriptsForFixedUpdate, script);
		}

		m_pendingUnregisterForFixedUpdate.clear();
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

void ScriptSystem::CallOnRender()
{
	for (const auto& script : m_scriptsForOnRender)
	{
		script->OnRender();
	}

	if (!m_pendingUnregisterForOnRender.empty())
	{
		for (const auto& script : m_pendingUnregisterForOnRender)
		{
			Util::OptimizedErase(m_scriptsForOnRender, script);
		}

		m_pendingUnregisterForOnRender.clear();
	}
}
