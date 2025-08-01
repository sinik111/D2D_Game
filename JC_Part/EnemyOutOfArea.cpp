#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/TextRenderer.h"

#include <iostream>

#include "EnemyOutOfArea.h"


void EnemyOutOfArea::Enter(FSMContext& context)
{
	m_Script->StopMoving();
}

void EnemyOutOfArea::Update(FSMContext& context) 
{		
	
	if (IsInCamera())
	{ 
		context.intParams[L"NextEnemyState"] = EnemyBase::INAREA; 
	}


	if (Vector2::Distance(Pos(), OriginPos()) > MaxRoamDistance())
	{
		context.intParams[L"NextEnemyState"] = EnemyBase::RETURN;
	}

}

void EnemyOutOfArea::Exit(FSMContext& context)
{

}