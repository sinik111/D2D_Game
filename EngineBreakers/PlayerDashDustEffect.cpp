#include "../D2DEngineLib/framework.h"
#include "PlayerDashDustEffect.h"

#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/BitmapRenderer.h"

void PlayerDashDustEffect::Initialize()
{
	auto br = GetGameObject()->AddComponent<BitmapRenderer>();
	br->SetSortOrder(1);
	br->SetColor({ 1.0f, 1.0f, 1.0f, 0.5f });

	m_animator = GetGameObject()->AddComponent<Animator>();

	m_animator->AddSpriteSheet(L"PlayerEffect/Dust_SPRITE_sprites.json");
	m_animator->AddAnimationClip(L"PlayerEffect/Dust_SPRITE_anim.json");
}

void PlayerDashDustEffect::Start()
{	
	m_animator->Play(L"Dust_SPRITE");

	m_direction = Random::Direction();
}

void PlayerDashDustEffect::Update()
{
	if (m_animator->IsFinished())
	{
		Destroy(GetGameObject());
	}

	Vector2 position = GetTransform()->GetLocalPosition();
	position += m_direction * 100.0f * MyTime::DeltaTime();
	GetTransform()->SetLocalPosition(position);
}
