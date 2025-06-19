#pragma once

#include "../D2DEngineLib/Script.h"

class TitleSceneChange :
	public Script
{
public:
    ~TitleSceneChange() override = default;

private:
    void Start() override;

    void ChangeScene();
};