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
	m_isStateChanged = false;
	Debug::Log("³Ë´Ù¿î--!!");
	m_Script->StopMoving();
	m_KnockdownTimer = 0.0f;

	if (m_Script->IsKnockback())
	{
		m_Script->StopKnockback();
	}
}


void EnemyKnockdown::Update(FSMContext& context)
{
	if (!m_Script) return;

	if (m_isStateChanged) return;

	CheckTargetDistance();
	CheckTargetInAtkRange();

	m_KnockdownTimer += MyTime::DeltaTime();

	if (m_Script->IsGrabbed())
	{
		Debug::Log("À¸¾Æ¾Ç ±×·¦ÀÌ´Ù--!!");
		
		m_Script->KnockdownAccumulated() = 0;

		IsKnockdown() = false;
		m_isStateChanged = true;
		context.intParams[L"NextEnemyState"] = EnemyBase::GRABBED;
		return;
	}


	if (m_KnockdownTimer >= 3.0f)
	{
		Debug::Log("³Ë´Ù¿îÇ®¸²--!!");
		
		IsKnockdown() = false;		
		m_isStateChanged = true;

		context.intParams[L"NextEnemyState"] = EnemyBase::SLOWTURN;
		return;
	}
}

void EnemyKnockdown::Exit(FSMContext& context)
{

}