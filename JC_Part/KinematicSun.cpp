#include "../D2DEngineLib/framework.h"
#include "KinematicSun.h"

#include "../D2DEngineLib/RigidBody2D.h"

void KinematicSun::Start()
{
	m_rigidBody = GetGameObject()->GetComponent<RigidBody2D>();
}

void KinematicSun::FixedUpdate()
{
	Vector2 velocity = Vector2::Zero;

	if (Input::IsKeyHeld(VK_UP))
	{
		velocity.y = 100.0f;
	}

	if (Input::IsKeyHeld(VK_DOWN))
	{
		velocity.y = -100.0f;
	}

	if (Input::IsKeyHeld(VK_LEFT))
	{
		velocity.x = -100.0f;
	}

	if (Input::IsKeyHeld(VK_RIGHT))
	{
		velocity.x = 100.0f;
	}

	m_rigidBody->SetVelocity(velocity);
}

void KinematicSun::Update()
{
	if (Input::IsKeyPressed('1'))
	{
		SceneManager::Get().ChangeScene(L"Title");
	}
}
