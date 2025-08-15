#pragma once
#include "../D2DEngineLib/Script.h"

class BitmapRenderer;
class InGameSettingController
	:public Script
{
	BitmapRenderer* bitmap = nullptr;
	void Start() override;
	void Update() override;

private:
	bool isOn = false;

public:
	bool GetIsOn() { return isOn; }
	void SetIsOn(bool value) { isOn = value; }
};

