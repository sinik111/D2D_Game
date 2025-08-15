#include "../D2DEngineLib/framework.h"

#include "../D2DEngineLib/Physics.h"
#include "../D2DEngineLib/ConeCollider2D.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/Script.h"

#include "EnemyDead.h"

void EnemyDead::Enter(FSMContext& context)
{
	m_isStateChanged = false;
	
	m_timeForHAIKU = 3.0f;

	m_Script->StopMoving();

	if (m_Script->IsKnockback())
	{
		m_Script->StopKnockback();
	}
}


void EnemyDead::Update(FSMContext& context)
{
	if (m_isStateChanged) return;

	m_timeForHAIKU -= MyTime::DeltaTime();

	if (m_timeForHAIKU <= MyMath::EPSILON)
	{
		m_isStateChanged = true;
		m_Script->DoDeleteThisObj() = true;
		context.intParams[L"NextEnemyState"] = EnemyBase::OUTOFAREA;
		return;
	}
}

void EnemyDead::Exit(FSMContext& context)
{

}