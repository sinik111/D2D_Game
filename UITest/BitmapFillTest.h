#pragma once

#include "../D2DEngineLib/Script.h"

class BitmapRenderer;

class BitmapFillTest :
	public Script
{
private:
	BitmapRenderer* m_bitmapRenderer = nullptr;
private:
	void Start() override;
	void Update() override;
};

