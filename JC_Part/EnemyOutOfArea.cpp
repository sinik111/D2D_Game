#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/TextRenderer.h"

#include "EnemyOutOfArea.h"


void EnemyOutOfArea::Enter(FSMContext& context)
{
	
}

void EnemyOutOfArea::Update(FSMContext& context) 
{	
	CheckCameraArea();

	if (IsInCamera())	{ context.intParams[L"NextEnemyState"] = EnemyBase::INAREA; }


}

void EnemyOutOfArea::Exit(FSMContext& context)
{

}