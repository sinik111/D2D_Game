#pragma once

#include "../D2DEngineLib/WinApp.h"

class UITestApp :
	public WinApp
{
public:
	void Initialize() override;

private:
	void MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

};