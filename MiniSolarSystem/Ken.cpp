#include "../D2DEngineLib/framework.h"
#include "Ken.h"

#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/PlayerInput.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/SceneManager.h"
#include "../D2DEngineLib/MyTime.h"

#include "Earth.h"

void Ken::Initialize()
{
	m_animator = GetGameObject()->GetComponent<Animator>();
}

void Ken::Start()
{
	PlayerInput* playerInput = GetGameObject()->GetComponent<PlayerInput>();
	playerInput->RegisterActionOnKey('Q', KeyState::Pressed, this, &Ken::BackDash);
	playerInput->RegisterActionOnKey('Q', KeyState::Released, this, &Ken::Idle);
	playerInput->RegisterActionOnKey('W', KeyState::Pressed, this, &Ken::FrontDash);
	playerInput->RegisterActionOnKey('W', KeyState::Released, this, &Ken::Idle);
	playerInput->RegisterActionOnKey('E', KeyState::Pressed, this, &Ken::Roll);
	playerInput->RegisterActionOnKey('R', KeyState::Pressed, this, &Ken::SpinningKick);
	playerInput->RegisterActionOnKey('1', KeyState::Pressed, this, &Ken::ChangeScene);

	m_animator = GetGameObject()->GetComponent<Animator>();
	m_animator->SetSpriteSheet(L"ken_sprites.json");
	m_animator->AddAnimationClip(L"ken_idle_anim.json");
	m_animator->AddAnimationClip(L"ken_front_dash_anim.json");
	m_animator->AddAnimationClip(L"ken_back_dash_anim.json");
	m_animator->AddAnimationClip(L"ken_roll_anim.json");
	m_animator->AddAnimationClip(L"ken_spinning_kick_anim.json");

	m_animator->AddActionOnEvent(L"FireEarth", this, &Ken::FireEarth);

	m_animator->Play(L"ken_idle");
}

void Ken::Update()
{
	if (m_animator->IsFinished())
	{
		if (m_moveDir == 1)
		{
			m_animator->Play(L"ken_front_dash");
		}
		else if (m_moveDir == -1)
		{
			m_animator->Play(L"ken_back_dash");
		}
		else
		{
			m_animator->Play(L"ken_idle");
		}
	}
}

void Ken::Idle()
{
	m_animator->Play(L"ken_idle");

	m_moveDir = 0;
}

void Ken::FrontDash()
{
	m_animator->Play(L"ken_front_dash");

	m_moveDir = 1;
}

void Ken::BackDash()
{
	m_animator->Play(L"ken_back_dash");

	m_moveDir = -1;
}

void Ken::Roll()
{
	m_animator->Play(L"ken_roll");
}

void Ken::SpinningKick()
{
	m_animator->Play(L"ken_spinning_kick");
}

void Ken::ChangeScene()
{
	SceneManager::Get().ChangeScene(L"Title");
}

void Ken::FireEarth()
{
	GameObject* go = CreateGameObject(L"Earth");
	go->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition() + Vector2(50.0f, 70.0f));
	Earth* earth = go->AddComponent<Earth>();
	earth->Fired();
}