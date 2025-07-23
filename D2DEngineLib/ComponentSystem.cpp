#include "pch.h"
#include "ComponentSystem.h"

ComponentSystem::ComponentSystem()
	: m_scriptSystem{ std::make_unique<ScriptSystem>() },
	m_rendererSystem{ std::make_unique<RendererSystem>() },
	m_playerInputSystem{ std::make_unique<PlayerInputSystem>() },
	m_animatorSystem{ std::make_unique<AnimatorSystem>() },
	m_transformSystem{ std::make_unique<TransformSystem>() },
	m_physicsSystem{ std::make_unique<PhysicsSystem>() },
	m_buttonSystem{ std::make_unique<ButtonSystem>() },
	m_particleSystem{ std::make_unique<ParticleSystem>() }
{
	
}

ComponentSystem& ComponentSystem::Get()
{
	static ComponentSystem s_instance;

	return s_instance;
}

ScriptSystem& ComponentSystem::Script() const
{
	return *m_scriptSystem.get();
}

RendererSystem& ComponentSystem::Renderer() const
{
	return *m_rendererSystem.get();
}

PlayerInputSystem& ComponentSystem::PlayerInput() const
{
	return *m_playerInputSystem.get();
}

AnimatorSystem& ComponentSystem::Animator() const
{
	return *m_animatorSystem.get();
}

TransformSystem& ComponentSystem::Transform() const
{
	return *m_transformSystem.get();
}

PhysicsSystem& ComponentSystem::Physics() const
{
	return *m_physicsSystem.get();
}

ButtonSystem& ComponentSystem::Button() const
{
	return *m_buttonSystem.get();
}

ParticleSystem& ComponentSystem::Particle() const
{
	return *m_particleSystem.get();
}
