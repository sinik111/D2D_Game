#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/TextRenderer.h"

#include "EnemyInArea.h"


void EnemyInArea::Enter(FSMContext& context)
{

}

void EnemyInArea::Update(FSMContext& context)
{
	CheckCameraArea();
	
	if (!IsInCamera())	{ context.intParams[L"NextEnemyState"] = OUTOFAREA; }

}

void EnemyInArea::Exit(FSMContext& context)
{

}