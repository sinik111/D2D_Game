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

	if (Input::IsKeyHeld(VK_UP))
	{
		velocity.y += 100.0f;
	}

	if (Input::IsKeyHeld(VK_DOWN))
	{
		velocity.y -= 100.0f;
	}

	if (Input::IsKeyHeld(VK_LEFT))
	{
		velocity.x -= 100.0f;
	}

	if (Input::IsKeyHeld(VK_RIGHT))
	{
		velocity.x += 100.0f;
	}

	m_rb2d->SetVelocity(velocity);
}
