#pragma once

#include "..\D2DEngineLib\Script.h"
#include "../D2DEngineLib/Delegate.h"

class Health;

class Moon :
    public Script
{
private:
    float m_speed = 0.0f;
    Health* m_health = nullptr;
    Delegate<const std::wstring&> m_onRotationChange;

private:
    void Start() override;
    void Update() override;
    void OnDestroy() override;

public:
    Delegate<const std::wstring&>& GetOnRotationChange();
};