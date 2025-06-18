#pragma once

#include <memory>

#include "ScriptSystem.h"
#include "BitmapRendererSystem.h"
#include "TextRendererSystem.h"

class ComponentSystem
{
private:
	std::unique_ptr<ScriptSystem> m_scriptSystem;
	std::unique_ptr<BitmapRendererSystem> m_bitmapRendererSystem;
	std::unique_ptr<TextRendererSystem> m_textRendererSystem;

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
	static ScriptSystem& Script();
	static BitmapRendererSystem& BitmapRenderer();
	static TextRendererSystem& TextRenderer();
};