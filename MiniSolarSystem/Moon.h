#pragma once

#include "..\D2DEngineLib\Script.h"

class Moon :
    public Script
{
private:
    float m_speed{};

public:
    ~Moon() override = default;

private:
    void Start() override;
    void Update() override;
};