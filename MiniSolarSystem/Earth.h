#pragma once

#include "..\D2DEngineLib\Script.h"
#include "../D2DEngineLib/Delegate.h"

class Health;

class Earth :
    public Script
{
private:
    float m_speed = 0.0f;
    Health* m_health = nullptr;
    Delegate<const std::wstring&> m_onRotationChange;

public:
    ~Earth() override;

private:
    void Start() override;
    void Update() override;

public:
    Delegate<const std::wstring&>& GetOnRotationChange();
};