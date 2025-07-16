#pragma once

#include "../D2DEngineLib/Script.h"

class JumpingText :
	public Script
{
private:
	bool m_gettingBigger = true;

private:
	void Initialize() override;
	void Update() override;
};