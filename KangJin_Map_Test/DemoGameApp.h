#pragma once
#include "../D2DEngineLib/WinApp.h"
class DemoGameApp : public WinApp
{
	void MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
public :
	void Initialize() override;
};

