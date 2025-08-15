#include "../D2DEngineLib/framework.h"

#include "../D2DEngineLib/Physics.h"
#include "../D2DEngineLib/ConeCollider2D.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/Script.h"
#include "../D2DEngineLib/BitmapRenderer.h"

#include "EnemyKnockdown.h"



void EnemyKnockdown::Enter(FSMContext& context)
{
	m_isStateChanged = false;
	Debug::Log("³Ë´Ù¿î--!!");
	m_Script->StopMoving();
	m_KnockdownTimer = 0.0f;

	m_grabIcon = m_Script->CreateGameObject(L"GrabIcon");
	m_grabIcon->GetTransform()->SetLocalPosition(context.transform->GetLocalPosition() + Vector2::Up * 100.0f);
	auto br = m_grabIcon->AddComponent<BitmapRenderer>(L"Image/UI/grabIcon.png");
	br->SetSortOrder(2);
	br->SetSpaceType(SpaceType::World);

	if (m_Script->m_enemyType == EnemyBase::BOSS)
	{
		m_knockdownDuration = 4.08f;
	}
	else
	{
		m_knockdownDuration = 3.0f;
	}

	if (m_Script->IsKnockback())
	{
		m_Script->StopKnockback();
	}
}


void EnemyKnockdown::Update(FSMContext& context)
{
	if (!m_Script) return;

	if (m_Script->m_isDead == true || m_Script->m_hp <= 0)
	{
		return;
	}

	if (m_isStateChanged) return;

	CheckTargetDistance();
	CheckTargetInAtkRange();
		
	m_KnockdownTimer += MyTime::DeltaTime();

	if (m_Script->IsGrabbed())
	{		
		m_Script->KnockdownAccumulated() = 0;

		IsKnockdown() = false;
		m_isStateChanged = true;
		context.intParams[L"NextEnemyState"] = EnemyBase::GRABBED;
		return;
	}
	

	if (m_KnockdownTimer >= m_knockdownDuration)
	{
		
		IsKnockdown() = false;		
		m_isStateChanged = true;

		context.intParams[L"NextEnemyState"] = EnemyBase::SLOWTURN;
		return;
	}
}

void EnemyKnockdown::Exit(FSMContext& context)
{
	if (m_grabIcon)
	{
		m_Script->Destroy(m_grabIcon);
		m_grabIcon = nullptr;
	}
}