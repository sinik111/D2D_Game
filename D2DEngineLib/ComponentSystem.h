#pragma once

#include "ScriptSystem.h"
#include "BitmapRendererSystem.h"
#include "TextRendererSystem.h"
#include "PlayerInputSystem.h"
#include "AnimatorSystem.h"
#include "TransformSystem.h"
#include "PhysicsSystem.h"

class Component;

class ComponentSystem
{
private:
	std::unique_ptr<ScriptSystem> m_scriptSystem;
	std::unique_ptr<BitmapRendererSystem> m_bitmapRendererSystem;
	std::unique_ptr<TextRendererSystem> m_textRendererSystem;
	std::unique_ptr<PlayerInputSystem> m_playerInputSystem;
	std::unique_ptr<AnimatorSystem> m_animatorSystem;
	std::unique_ptr<TransformSystem> m_transformSystem;
	std::unique_ptr<PhysicsSystem> m_physicsSystem;

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
	BitmapRendererSystem& BitmapRenderer() const;
	TextRendererSystem& TextRenderer() const;
	PlayerInputSystem& PlayerInput() const;
	AnimatorSystem& Animator() const;
	TransformSystem& Transform() const;
	PhysicsSystem& Physics() const;
};