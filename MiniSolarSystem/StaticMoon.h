#pragma once

#include "../D2DEngineLib/Script.h"

class StaticMoon :
	public Script
{
public:
	~StaticMoon();

private:
	void OnTriggerEnter(const Collision& collision) override;
};