#pragma once

#include "../D2DEngineLib/WinApp.h"

class DemoGameApp :
	public WinApp
{
public:
	~DemoGameApp() override = default;

public:
	void Initialize();
	void Shutdown() override;

	void MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
};