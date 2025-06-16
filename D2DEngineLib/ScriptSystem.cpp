#include "pch.h"
#include "ScriptSystem.h"

#include "ContainerUtility.h"
#include "Script.h"

void ScriptSystem::Register(Script* script)
{
	m_scripts.push_back(script);
	m_scriptsForStart.push_back(script);
	m_scriptsForUpdate.push_back(script);
	m_scriptsForLateUpdate.push_back(script);
}

void ScriptSystem::Unregister(Script* script)
{
	Util::OptimizedErase(m_scripts, script);
	Util::OptimizedErase(m_scriptsForStart, script);
	Util::OptimizedErase(m_scriptsForUpdate, script);
	Util::OptimizedErase(m_scriptsForLateUpdate, script);
}

void ScriptSystem::UnregisterUpdate(Script* script)
{
	Util::OptimizedErase(m_scriptsForUpdate, script);
}

void ScriptSystem::UnregisterLateUpdate(Script* script)
{
	Util::OptimizedErase(m_scriptsForLateUpdate, script);
}

void ScriptSystem::UpdateSystem()
{
	CallStart();
	CallUpdate();
	CallLateUpdate();
}

void ScriptSystem::CallStart()
{
	if (!m_scriptsForStart.empty())
	{
		for (auto script : m_scriptsForStart)
		{
			script->Start();
		}

		m_scriptsForStart.clear();
	}
}

void ScriptSystem::CallUpdate()
{
	for (auto script : m_scriptsForUpdate)
	{
		script->Update();
	}
}

void ScriptSystem::CallLateUpdate()
{
	for (auto script : m_scriptsForLateUpdate)
	{
		script->LateUpdate();
	}
}