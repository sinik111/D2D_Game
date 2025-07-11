#pragma once

#include "../D2DEngineLib/Script.h"
#include "../D2DEngineLib/Delegate.h"

class Health;

class Earth :
    public Script
{
private:
    Health* m_health = nullptr;
    Delegate<const std::wstring&> m_onRotationChange;
    float m_speed = 0.0f;
    float m_destroyTimer = 0.0f;
    bool m_isFired = false;

private:
    void Start() override;
    void Update() override;
    void OnDestroy() override;

public:
    Delegate<const std::wstring&>& GetOnRotationChange();
    void Fired();
};