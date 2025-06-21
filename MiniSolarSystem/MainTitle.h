#pragma once

#include "../D2DEngineLib/Scene.h"

class MainTitle :
	public Scene
{
public:
	~MainTitle() override;

public:
	void Load() override;
	void Enter() override;
	void Unload() override;
};