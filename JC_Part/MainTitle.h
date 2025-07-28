#pragma once

#include "../D2DEngineLib/Scene.h"

class MainTitle :
	public Scene
{
public:
	// [Tip] Scene의 Enter는 순수 가상함수여서 필수로 구현해야함

	void Enter() override;
};