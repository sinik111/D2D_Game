#pragma once

#include "../D2DEngineLib/WinApp.h"

class DemoGameApp :
	public WinApp
{
public:
	void Initialize() override;

private:
	void MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
};