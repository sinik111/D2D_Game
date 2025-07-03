#include "../D2DEngineLib/framework.h"
#include "Ken.h"

#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/PlayerInput.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/SceneManager.h"
#include "../D2DEngineLib/MyTime.h"

#include "Earth.h"
#include "FSMContext.h"

void Ken::Initialize()
{
	m_animator = GetGameObject()->GetComponent<Animator>();
	m_kenFSM = std::make_unique<KenFSM>();
}

void Ken::Start()
{
	PlayerInput* playerInput = GetGameObject()->GetComponent<PlayerInput>();
	playerInput->RegisterDirectionAction(DirectionInputType::Arrow, this, &Ken::ArrowInput);
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

	m_floatParams[L"HorizontalInput"] = 0.0f;
	m_triggerParams[L"Roll"] = false;
	m_triggerParams[L"SpinningKick"] = false;
}

void Ken::Update()
{
	FSMContext context{};
	context.transform = GetTransform();
	context.animator = m_animator;
	context.floatParams = &m_floatParams;
	context.triggerParams = &m_triggerParams;

	m_kenFSM->Update(context);

	for (auto& pair : m_triggerParams)
	{
		pair.second = false;
	}
}

void Ken::ArrowInput(Vector2 input)
{
	m_floatParams[L"HorizontalInput"] = input.GetX();
}

void Ken::Roll()
{
	m_triggerParams[L"Roll"] = true;
}

void Ken::SpinningKick()
{
	m_triggerParams[L"SpinningKick"] = true;
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