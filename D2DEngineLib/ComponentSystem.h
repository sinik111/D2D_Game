#pragma once

#include <memory>

#include "ScriptSystem.h"
#include "BitmapRendererSystem.h"
#include "TextRendererSystem.h"
#include "PlayerInputSystem.h"

class ComponentSystem
{
private:
	std::unique_ptr<ScriptSystem> m_scriptSystem;
	std::unique_ptr<BitmapRendererSystem> m_bitmapRendererSystem;
	std::unique_ptr<TextRendererSystem> m_textRendererSystem;
	std::unique_ptr<PlayerInputSystem> m_playerInputSystem;

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
	ScriptSystem& Script();
	BitmapRendererSystem& BitmapRenderer();
	TextRendererSystem& TextRenderer();
	PlayerInputSystem& PlayerInput();
};