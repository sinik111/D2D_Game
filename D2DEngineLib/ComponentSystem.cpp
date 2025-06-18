#include "pch.h"
#include "ComponentSystem.h"

ComponentSystem::ComponentSystem()
	: m_scriptSystem{ std::make_unique<ScriptSystem>() },
	m_bitmapRendererSystem{ std::make_unique<BitmapRendererSystem>() },
	m_textRendererSystem{ std::make_unique<TextRendererSystem>() }
{
	
}

ComponentSystem& ComponentSystem::Get()
{
	static ComponentSystem s_instance;

	return s_instance;
}

ScriptSystem& ComponentSystem::Script()
{
	return *Get().m_scriptSystem.get();
}

BitmapRendererSystem& ComponentSystem::BitmapRenderer()
{
	return *Get().m_bitmapRendererSystem.get();
}

TextRendererSystem& ComponentSystem::TextRenderer()
{
	return *Get().m_textRendererSystem.get();
}