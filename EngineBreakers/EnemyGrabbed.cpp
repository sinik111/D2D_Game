#include "../D2DEngineLib/framework.h"

#include "../D2DEngineLib/Physics.h"
#include "../D2DEngineLib/ConeCollider2D.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/Script.h"

#include "EnemyGrabbed.h"

void EnemyGrabbed::Enter(FSMContext& context)
{	
	m_isStateChanged = false;
	m_grabTimer = m_Script->GrabTimer();

	m_Script->StopMoving();
	

	if (m_Script->IsKnockback())
	{
		m_Script->StopKnockback();
	}
}


void EnemyGrabbed::Update(FSMContext& context)
{
	if (m_Script->m_isDead == true || m_Script->m_hp <= 0)
	{		
		return;
	}


	if (m_isStateChanged) return;


	
	m_grabTimer -= MyTime::DeltaTime();


	if (m_grabTimer <= MyMath::EPSILON)
	{		
		m_isStateChanged = true;
		m_Script->IsGrabbed() = false;
		context.intParams[L"NextEnemyState"] = EnemyBase::SLOWTURN;
		return;
	}

	CheckTargetDistance();
	CheckTargetInAtkRange();
}

void EnemyGrabbed::Exit(FSMContext& context)
{

}