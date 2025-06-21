#pragma once

#include "..\D2DEngineLib\Script.h"

class Earth :
    public Script
{
private:
    float m_speed{};

public:
    ~Earth() override = default;

private:
    void Start() override;
    void Update() override;
};