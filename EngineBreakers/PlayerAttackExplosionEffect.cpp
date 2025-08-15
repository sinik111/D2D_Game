#include "../D2DEngineLib/framework.h"
#include "PlayerAttackExplosionEffect.h"

#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/BitmapRenderer.h"

void PlayerAttackExplosionEffect::Initialize()
{
	auto br = GetGameObject()->AddComponent<BitmapRenderer>();
	br->SetSortOrder(1);
	br->SetColor({ 1.0f, 1.0f, 1.0f, 0.5f });

	m_animator = GetGameObject()->AddComponent<Animator>();

	m_animator->AddSpriteSheet(L"PlayerEffect/Explosion_sprites.json");
	m_animator->AddAnimationClip(L"PlayerEffect/Explosion_e_anim.json");
}

void PlayerAttackExplosionEffect::Start()
{
	m_animator->Play(L"Explosion_e");
}

void PlayerAttackExplosionEffect::Update()
{
	if (m_animator->IsFinished())
	{
		Destroy(GetGameObject());
	}
}
