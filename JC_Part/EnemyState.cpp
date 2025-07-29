#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/TextRenderer.h"

#include "EnemyState.h"


void EnemyState::Enter(FSMContext& context)
{
	SetDirection(context);
}

void EnemyState::Update(FSMContext& context)
{
	SetDirection(context);
}

void EnemyState::Exit(FSMContext& context)
{

}


void EnemyState::SetDirection(FSMContext& context)
{
	std::wstring temp = context.currentStateName;

	curDir = context.intParams[L"Direction"];	

	//if(context.textRenderer)
	//	context.textRenderer->SetText(temp + L" " + std::to_wstring(curDir));

	if (curDir != prevDir)
	{
		prevDir = curDir;

		context.animator->Play(animPath[curDir]);

		
	}
}