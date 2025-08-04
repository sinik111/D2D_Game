#include "../D2DEngineLib/framework.h"
#include "PlayerController.h"
#include "../D2DEngineLib/RigidBody2D.h"

void PlayerController::Start()
{
	m_rb2d = GetGameObject()->GetComponent<RigidBody2D>();
}

void PlayerController::FixedUpdate()
{
	Vector2 velocity = Vector2::Zero;

	if (Input::IsKeyHeld('W'))
	{
		velocity.y += 200.0f;
	}
	
	if (Input::IsKeyHeld('S'))
	{
		velocity.y -= 200.0f;
	}

	if (Input::IsKeyHeld('A'))
	{
		velocity.x -= 200.0f;
	}

	if (Input::IsKeyHeld('D'))
	{
		velocity.x += 200.0f;
	}
	m_rb2d->SetVelocity(velocity);
}