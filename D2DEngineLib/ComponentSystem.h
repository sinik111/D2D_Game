#pragma once

#include <memory>

#include "ScriptSystem.h"
#include "BitmapRendererSystem.h"
#include "TextRendererSystem.h"
#include "PlayerInputSystem.h"
#include "AnimatorSystem.h"

class Component;

class ComponentSystem
{
private:
	std::unique_ptr<ScriptSystem> m_scriptSystem;
	std::unique_ptr<BitmapRendererSystem> m_bitmapRendererSystem;
	std::unique_ptr<TextRendererSystem> m_textRendererSystem;
	std::unique_ptr<PlayerInputSystem> m_playerInputSystem;
	std::unique_ptr<AnimatorSystem> m_animatorSystem;

	std::unordered_set<Component*> m_validComponents;

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
	AnimatorSystem& Animator();

public:
	void RegisterValidCheck(Component* component);
	void UnregisterValidCheck(Component* component);
	bool IsValid(Component* component) const;
};