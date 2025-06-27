#pragma once

#include "../D2DEngineLib/Scene.h"

class SolarSystem :
	public Scene
{
public:
	~SolarSystem() override;

public:
	void Load() override;
	void Enter() override;
	void Unload() override;
};