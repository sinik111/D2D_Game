#include "../D2DEngineLib/framework.h"

#include "../D2DEngineLib/Physics.h"
#include "../D2DEngineLib/ConeCollider2D.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/Script.h"

#include "EnemyKnockdown.h"



void EnemyKnockdown::Enter(FSMContext& context)
{
	Debug::Log("³Ë´Ù¿î");
	m_Script->StopMoving();
	m_KnockdownTimer = 0.0f;

	if (m_Script->IsKnockback())
	{
		m_Script->StopKnockback();
	}	
}


void EnemyKnockdown::Update(FSMContext& context)
{
	m_KnockdownTimer += MyTime::DeltaTime();

	if (m_KnockdownTimer >= 2.0f)
	{
		Debug::Log("³Ë´Ù¿îÇ®¸²");				
		m_Script->AheadToTarget();
		context.intParams[L"NextEnemyState"] = EnemyBase::ENGAGE;		
	}
}

void EnemyKnockdown::Exit(FSMContext& context)
{
	
}