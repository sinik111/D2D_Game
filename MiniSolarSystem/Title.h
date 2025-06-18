#pragma once

#include "..\D2DEngineLib\Script.h"

class Title :
    public Script
{
public:
    ~Title() override = default;

private:
    void Start() override;
};