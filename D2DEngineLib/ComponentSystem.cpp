#include "pch.h"
#include "ComponentSystem.h"

ComponentSystem::ComponentSystem()
	: m_scriptSystem{ std::make_unique<ScriptSystem>() },
	m_bitmapRendererSystem{ std::make_unique<BitmapRendererSystem>() },
	m_textRendererSystem{ std::make_unique<TextRendererSystem>() },
	m_playerInputSystem{ std::make_unique<PlayerInputSystem>() },
	m_animatorSystem{ std::make_unique<AnimatorSystem>() }
{
	
}

ComponentSystem& ComponentSystem::Get()
{
	static ComponentSystem s_instance;

	return s_instance;
}

ScriptSystem& ComponentSystem::Script()
{
	return *m_scriptSystem.get();
}

BitmapRendererSystem& ComponentSystem::BitmapRenderer()
{
	return *m_bitmapRendererSystem.get();
}

TextRendererSystem& ComponentSystem::TextRenderer()
{
	return *m_textRendererSystem.get();
}

PlayerInputSystem& ComponentSystem::PlayerInput()
{
	return *m_playerInputSystem.get();
}

AnimatorSystem& ComponentSystem::Animator()
{
	return *m_animatorSystem.get();
}