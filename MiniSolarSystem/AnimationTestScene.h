#pragma once

#include "../D2DEngineLib/Scene.h"

class AnimationTestScene :
	public Scene
{
public:
	~AnimationTestScene() override;

public:
	void Load() override;
	void Enter() override;
	void Unload() override;
};