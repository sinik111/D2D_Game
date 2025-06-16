#pragma once

#include <memory>

#include "TransformSystem.h"
#include "ScriptSystem.h"

class ComponentSystem
{
private:
	std::unique_ptr<TransformSystem> m_transformSystem;
	std::unique_ptr<ScriptSystem> m_scriptSystem;

private:
	ComponentSystem();
	ComponentSystem(const ComponentSystem&) = delete;
	ComponentSystem& operator=(const ComponentSystem&) = delete;
	ComponentSystem(ComponentSystem&&) = delete;
	ComponentSystem& operator=(ComponentSystem&&) = delete;
	~ComponentSystem() = default;

private:
	static ComponentSystem& Get();

public:
	static TransformSystem& Transform();
	static ScriptSystem& Script();

public:
	void UpdateSystems();
};