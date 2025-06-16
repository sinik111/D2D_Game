#include "pch.h"
#include "ScriptSystem.h"

#include "ContainerUtility.h"
#include "Script.h"

void ScriptSystem::Register(Script* script)
{
	m_scripts.push_back(script);
	m_startScripts.push_back(script);
	m_updateScripts.push_back(script);
	m_lateUpdateScripts.push_back(script);
}

void ScriptSystem::Unregister(Script* script)
{
	OptimizedErase(m_scripts, script);
	OptimizedErase(m_startScripts, script);
	OptimizedErase(m_updateScripts, script);
	OptimizedErase(m_lateUpdateScripts, script);
}

void ScriptSystem::UnregisterUpdate(Script* script)
{
	OptimizedErase(m_updateScripts, script);
}

void ScriptSystem::UnregisterLateUpdate(Script* script)
{
	OptimizedErase(m_lateUpdateScripts, script);
}

void ScriptSystem::UpdateSystem()
{
	StartScripts();
	UpdateScripts();
	LateUpdateScripts();
}

void ScriptSystem::StartScripts()
{
	if (!m_startScripts.empty())
	{
		for (auto script : m_startScripts)
		{
			script->Start();
		}

		m_startScripts.clear();
	}
}

void ScriptSystem::UpdateScripts()
{
	for (auto script : m_updateScripts)
	{
		script->Update();
	}
}

void ScriptSystem::LateUpdateScripts()
{
	for (auto script : m_lateUpdateScripts)
	{
		script->LateUpdate();
	}
}