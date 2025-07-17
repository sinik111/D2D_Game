#pragma once

#include "../D2DEngineLib/Script.h"

class StaticMoon :
	public Script
{
private:
	void OnTriggerEnter(const Collision& collision) override;
};