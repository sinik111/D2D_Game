#pragma once

#include "../D2DEngineLib/Scene.h"

class MainTitle :
	public Scene
{
public:
	MainTitle() = default;
	~MainTitle() override;

public:
	void Load() override;
	void Enter() override;
	void Exit() override;
	void Unload() override;
};