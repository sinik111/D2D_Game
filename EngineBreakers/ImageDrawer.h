#pragma once

#include "../D2DEngineLib/Script.h"
#include "../D2DEngineLib/FSM.h"
#include "../D2DEngineLib/FSMContext.h"

class PlayerInput;
class Animator;
class RigidBody2D;
class TextRenderer;
class BoxCollider2D;
class CircleCollider;

class ImageDrawer : public Script
{
public:
	ImageDrawer()
	{
	}

	virtual ~ImageDrawer()
	{
	}

protected:
	Animator* m_animator = nullptr;
	RigidBody2D* m_rigidBody = nullptr;
	TextRenderer* m_textRenderer = nullptr;

	//BoxCollider2D* m_collider = nullptr;
	//CircleCollider* m_circleCollider = nullptr;

	//FSM m_fsm;
	//FSMContext m_context;

	float m_lifeTimer = 1.0f;

protected:
	virtual void Initialize() override;
	virtual void Start() override;	
	virtual void Update() override;
	//virtual void OnDestroy() override;
	
	//virtual void OnTriggerEnter(const Collision& collision) override;
	//virtual void OnTriggerExit(const Collision& collision) override;

public:
	//GameObject* m_camera = nullptr;
	//GameObject* m_player = nullptr;
	//GameObject* m_enemyThis = nullptr;


};

