#pragma once

#include "../D2DEngineLib/Scene.h"

class SolarSystem :
	public Scene
{
public:
	SolarSystem() = default;
	virtual ~SolarSystem();

public:
	void Load() override;
	void Enter() override;
	void Exit() override;
	void Unload() override;

private:
	void LoadResources();
};