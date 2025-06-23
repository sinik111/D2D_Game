#pragma once

#include "..\D2DEngineLib\Script.h"
#include "../D2DEngineLib/Delegate.h"

class Health;

class Earth :
    public Script
{
private:
    float m_speed{};
    Health* m_health;
    Delegate<float> m_onRotationChange;

public:
    ~Earth() override;

private:
    void Start() override;
    void Update() override;

public:
    Delegate<float>& GetOnRotationChange();
};