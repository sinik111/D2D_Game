#include "../D2DEngineLib/framework.h"
#include "Ken.h"

#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/PlayerInput.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/SceneManager.h"
#include "../D2DEngineLib/MyTime.h"

#include "Earth.h"
#include "FSMContext.h"
#include "Health.h"

void Ken::Initialize()
{
	m_animator = GetGameObject()->GetComponent<Animator>();
	m_health = GetGameObject()->GetComponent<Health>();
}

void Ken::Start()
{
	PlayerInput* playerInput = GetGameObject()->GetComponent<PlayerInput>();
	playerInput->RegisterDirectionAction(DirectionInputType::Arrow, this, &Ken::ArrowInput);
	playerInput->RegisterActionOnKey('Q', KeyState::Pressed, this, &Ken::Roll);
	playerInput->RegisterActionOnKey('W', KeyState::Pressed, this, &Ken::SpinningKick);
	playerInput->RegisterActionOnKey('E', KeyState::Pressed, this, &Ken::TakeDamage);
	playerInput->RegisterActionOnKey('R', KeyState::Pressed, this, &Ken::Revive);

	playerInput->RegisterActionOnKey('1', KeyState::Pressed, this, &Ken::ChangeScene);

	m_animator->SetSpriteSheet(L"ken_sprites.json");
	m_animator->AddAnimationClip(L"ken_idle_anim.json");
	m_animator->AddAnimationClip(L"ken_front_dash_anim.json");
	m_animator->AddAnimationClip(L"ken_back_dash_anim.json");
	m_animator->AddAnimationClip(L"ken_roll_anim.json");
	m_animator->AddAnimationClip(L"ken_spinning_kick_anim.json");
	m_animator->AddAnimationClip(L"ken_hurt_anim.json");
	m_animator->AddAnimationClip(L"ken_dead_anim.json");
	m_animator->AddAnimationClip(L"ken_revive_anim.json");

	m_animator->AddActionOnEvent(L"FireEarth", this, &Ken::FireEarth);

	m_context.gameObject = GetGameObject();
	m_context.animator = m_animator;
	m_context.transform = GetTransform();
	m_context.bitmapRenderer = GetGameObject()->GetComponent<BitmapRenderer>();
	m_context.floatParams[L"HorizontalInput"] = 0.0f;
	m_context.triggerParams[L"Roll"] = false;
	m_context.triggerParams[L"SpinningKick"] = false;
	m_context.triggerParams[L"Hurt"] = false;
	m_context.boolParams[L"IsDead"] = false;
	m_context.triggerParams[L"Revive"] = false;

	m_kenFSM = std::make_unique<KenFSM>(m_context);

	m_health->SetHp(50, 50);
}

void Ken::Update()
{
	m_kenFSM->Update(m_context);
}

void Ken::ArrowInput(Vector2 input)
{
	m_context.floatParams[L"HorizontalInput"] = input.x;
}

void Ken::Roll()
{
	m_context.triggerParams[L"Roll"] = true;
}

void Ken::SpinningKick()
{
	m_context.triggerParams[L"SpinningKick"] = true;
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

void Ken::TakeDamage()
{
	m_health->TakeDamage(10, false);

	if (m_health->GetHp() == 0)
	{
		m_context.boolParams[L"IsDead"] = true;
	}
	else
	{
		m_context.triggerParams[L"Hurt"] = true;
	}
}

void Ken::Revive()
{
	m_context.triggerParams[L"Revive"] = true;
	m_context.boolParams[L"IsDead"] = false;
}