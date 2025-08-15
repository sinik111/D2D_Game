#include "../D2DEngineLib/framework.h"

#include <limits>
#include <iostream>

#include "../D2DEngineLib/SceneManager.h"
#include "../D2DEngineLib/Physics.h"
#include "../D2DEngineLib/ConeCollider2D.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/BoxCollider2D.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/TextRenderer.h"

#include "../D2DEngineLib/Script.h"

#include "../D2DEngineLib/Random.h"

#include <cmath>    
#include <random>   
#include <chrono>   
#include <thread>   

#include "EnemyOutOfArea.h"
#include "EnemyInArea.h"
#include "EnemyEngage.h"
#include "EnemyOnAttack.h"
#include "EnemyReturn.h"
#include "EnemyOnEvade.h"

//#include "TempEnemyAttack.h"

#include "Player.h"
#include "EnemyBase.h"
#include "EnemyInteract.h"







#include "../D2DEngineLib/framework.h"

#include <limits>
#include <iostream>

#include "../D2DEngineLib/SceneManager.h"
#include "../D2DEngineLib/Physics.h"
#include "../D2DEngineLib/ConeCollider2D.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/BoxCollider2D.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/TextRenderer.h"

#include "../D2DEngineLib/Script.h"

#include "../D2DEngineLib/Random.h"

#include <cmath>    
#include <random>   
#include <chrono>   
#include <thread>   

#include "EnemyOutOfArea.h"
#include "EnemyInArea.h"
#include "EnemyEngage.h"
#include "EnemyOnAttack.h"
#include "EnemyReturn.h"
#include "EnemyOnEvade.h"

//#include "TempEnemyAttack.h"

#include "Player.h"
#include "EnemyBase.h"
#include "EnemyInteract.h"




//현재 Parried함수의 사양은 HitEnemy와 완전 동일. 충돌 대상이 공격 오브젝트냐 Enemy오브젝트냐 구분하는 기능만 다름.


void EnemyInteract::EnemyUnderAttack()
{
	if (!m_owner) return;
	m_owner->KdPointResetTimer() = 0.0f;
	m_owner->IsUnderAttack() = true;
	m_owner->UnderAttackTimer() = 0.0f;
}


void EnemyInteract::AddKnockdownValue(const int& value)
{
	if (!m_owner) return;
	m_owner->KnockdownAccumulated() += value;
}


void EnemyInteract::CheckKnock()
{
	if (!m_owner) return;

	m_owner->EnemyIA()->KnockBack(10.0f, 5.0f);
	if (m_owner->KnockdownAccumulated() >= m_owner->KnockdownResist())
	{
		m_owner->IsKnockdown() = true;
	}
	//else if (m_owner->KnockdownAccumulated() >= m_owner->KnockbackResist())
	//{
	//	m_owner->EnemyIA()->KnockBack(200.0f, 5.0f);
	//}
}

void EnemyInteract::KnockBack(const float& dist = 200.0f, const float& speed = 5.0f)
{
	if (!m_owner) return;

	if (!m_owner->IsKnockback())
	{
		m_owner->StopMoving();
	}

	m_owner->IsKnockback() = true;
	m_owner->KnockbackCurrentTimer() = 0.0f;


	const Vector2 enemyPos = m_owner->Pos();
	const Vector2 playerPos = m_owner->TargetPos();
	Vector2 knockBackDirection = enemyPos - playerPos;

	knockBackDirection.Normalize();
	m_owner->KnockbackDirection() = knockBackDirection;

	m_owner->AheadDirection() = knockBackDirection * -1.0f;
	m_owner->SetAngleByAheadDirection(m_owner->AheadDirection());

	m_owner->KnockbackSpeed() = dist * speed;
}

void EnemyInteract::DealDmg(const int& value)
{
	if (!m_owner) return;
	m_owner->Hp() -= value;
}

