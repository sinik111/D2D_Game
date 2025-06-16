#include "pch.h"
#include "ComponentSystem.h"

ComponentSystem::ComponentSystem()
	: m_transformSystem{ std::make_unique<TransformSystem>() },
	m_scriptSystem{ std::make_unique<ScriptSystem>() }
{
	
}

ComponentSystem& ComponentSystem::Get()
{
	static ComponentSystem s_instance;

	return s_instance;
}

TransformSystem& ComponentSystem::Transform()
{
	return *Get().m_transformSystem.get();
}

ScriptSystem& ComponentSystem::Script()
{
	return *Get().m_scriptSystem.get();
}

void ComponentSystem::UpdateSystems()
{
	//m_transformSystem
	m_scriptSystem->UpdateSystem();
}
