#include "pch.h"
#include "ComponentSystem.h"

ComponentSystem::ComponentSystem()
	: m_scriptSystem{ std::make_unique<ScriptSystem>() },
	m_bitmapRendererSystem{ std::make_unique<BitmapRendererSystem>() },
	m_textRendererSystem{ std::make_unique<TextRendererSystem>() },
	m_playerInputSystem{ std::make_unique<PlayerInputSystem>() },
	m_animatorSystem{ std::make_unique<AnimatorSystem>() },
	m_transformSystem{ std::make_unique<TransformSystem>() },
	m_physicsSystem{ std::make_unique<PhysicsSystem>() }
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

BitmapRendererSystem& ComponentSystem::BitmapRenderer() const
{
	return *m_bitmapRendererSystem.get();
}

TextRendererSystem& ComponentSystem::TextRenderer() const
{
	return *m_textRendererSystem.get();
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
