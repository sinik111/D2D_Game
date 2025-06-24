#pragma once

#include "../D2DEngineLib/Script.h"

class TitleSceneChange :
	public Script
{
private:
    void Start() override;

    void ChangeScene();
};