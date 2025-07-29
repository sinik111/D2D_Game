#pragma once
#include "../D2DEngineLib/IState.h"

class EnemyState 
	: public IState
{
public:
	void Enter(FSMContext& context) override;
	void Update(FSMContext& context) override;

	void Exit(FSMContext& context) override;

	virtual void SetDirection(FSMContext& context);

	int prevDir = 0;
	int curDir = 0;

	std::wstring animPath[10] = {
		L"",
		L"",
		L"",
		L""
		L"",
		L"",
		L"",
		L"",
		L"",
		L"",
	};

};

