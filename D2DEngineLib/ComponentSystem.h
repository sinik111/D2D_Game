#pragma once

#include "ScriptSystem.h"
#include "RendererSystem.h"
#include "PlayerInputSystem.h"
#include "AnimatorSystem.h"
#include "TransformSystem.h"
#include "PhysicsSystem.h"
#include "ButtonSystem.h"

class Component;

class ComponentSystem
{
private:
	std::unique_ptr<ScriptSystem> m_scriptSystem;
	std::unique_ptr<RendererSystem> m_rendererSystem;
	std::unique_ptr<PlayerInputSystem> m_playerInputSystem;
	std::unique_ptr<AnimatorSystem> m_animatorSystem;
	std::unique_ptr<TransformSystem> m_transformSystem;
	std::unique_ptr<PhysicsSystem> m_physicsSystem;
	std::unique_ptr<ButtonSystem> m_buttonSystem;

private:
	ComponentSystem();
	ComponentSystem(const ComponentSystem&) = delete;
	ComponentSystem& operator=(const ComponentSystem&) = delete;
	ComponentSystem(ComponentSystem&&) = delete;
	ComponentSystem& operator=(ComponentSystem&&) = delete;
	~ComponentSystem() = default;

public:
	static ComponentSystem& Get();

public:
	ScriptSystem& Script() const;
	RendererSystem& Renderer() const;
	PlayerInputSystem& PlayerInput() const;
	AnimatorSystem& Animator() const;
	TransformSystem& Transform() const;
	PhysicsSystem& Physics() const;
	ButtonSystem& Button() const;
};