#pragma once

#include "../D2DEngineLib/Script.h"

class MainSceneChangeButton :
	public Script
{
private:
	void Start() override;

private:
	void ChangeScene();
};