#include "../D2DEngineLib/framework.h"
#include "Ken.h"

#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/PlayerInput.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/SceneManager.h"
#include "../D2DEngineLib/MyTime.h"

void Ken::Initialize()
{
	m_animator = GetGameObject()->GetComponent<Animator>();
}

void Ken::Start()
{
	PlayerInput* playerInput = GetGameObject()->GetComponent<PlayerInput>();
	if (!Component::IsValid(playerInput))
	{
		return;
	}

	playerInput->RegisterActionOnKey('Q', KeyState::Pressed, this, &Ken::FrontDash);
	playerInput->RegisterActionOnKey('Q', KeyState::Released, this, &Ken::Idle);
	playerInput->RegisterActionOnKey('W', KeyState::Pressed, this, &Ken::BackDash);
	playerInput->RegisterActionOnKey('W', KeyState::Released, this, &Ken::Idle);
	playerInput->RegisterActionOnKey('E', KeyState::Pressed, this, &Ken::Roll);
	playerInput->RegisterActionOnKey('R', KeyState::Pressed, this, &Ken::SpinningKick);
	playerInput->RegisterActionOnKey('1', KeyState::Pressed, this, &Ken::ChangeScene);

	m_animator = GetGameObject()->GetComponent<Animator>();
	m_animator->SetSpriteSheet(L"ken_sprites.json");
	m_animator->AddAnimationClip(L"Idle", L"ken_idle_anim.json");
	m_animator->AddAnimationClip(L"FrontDash", L"ken_front_dash_anim.json");
	m_animator->AddAnimationClip(L"BackDash", L"ken_back_dash_anim.json");
	m_animator->AddAnimationClip(L"Roll", L"ken_roll_anim.json");
	m_animator->AddAnimationClip(L"SpinningKick", L"ken_spinning_kick_anim.json");

	ChangeState(KenState::IDLE);
}

void Ken::Update()
{
	if (m_animator->IsFinished())
	{
		if (m_moveDir == 1)
		{
			ChangeState(KenState::FRONT_DASH);
		}
		else if (m_moveDir == -1)
		{
			ChangeState(KenState::BACK_DASH);
		}
		else
		{
			ChangeState(KenState::IDLE);
		}
	}
}

void Ken::ChangeState(KenState state)
{
	switch (state)
	{
	case KenState::IDLE:
		m_animator->Play(L"Idle");
		break;

	case KenState::FRONT_DASH:
		m_animator->Play(L"FrontDash");
		break;

	case KenState::BACK_DASH:
		m_animator->Play(L"BackDash");
		break;

	case KenState::ROLL:
		m_animator->Play(L"Roll");
		break;

	case KenState::SPINNING_KICK:
		m_animator->Play(L"SpinningKick");
		break;
	}
}

void Ken::Idle()
{
	if (m_animator->GetCurrentClipName() != L"Roll" &&
		m_animator->GetCurrentClipName() != L"SpinningKick")
	{
		ChangeState(KenState::IDLE);
	}

	m_moveDir = 0;
}

void Ken::FrontDash()
{
	if (m_animator->GetCurrentClipName() != L"Roll" &&
		m_animator->GetCurrentClipName() != L"SpinningKick")
	{
		ChangeState(KenState::FRONT_DASH);
	}

	m_moveDir = 1;
}

void Ken::BackDash()
{
	if (m_animator->GetCurrentClipName() != L"Roll" &&
		m_animator->GetCurrentClipName() != L"SpinningKick")
	{
		ChangeState(KenState::BACK_DASH);
	}

	m_moveDir = -1;
}

void Ken::Roll()
{
	ChangeState(KenState::ROLL);
}

void Ken::SpinningKick()
{
	ChangeState(KenState::SPINNING_KICK);
}

void Ken::ChangeScene()
{
	SceneManager::Get().ChangeScene(L"Title");
}
