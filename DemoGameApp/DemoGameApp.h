#pragma once

#include "../D2DEngineLib/WinApp.h"

class DemoGameApp :
	public WinApp
{
private:
	Microsoft::WRL::ComPtr<IWICImagingFactory> m_pWICImagingFactory;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_pD2DBitmapFromFile;

public:
	DemoGameApp() = default;
	~DemoGameApp() = default;

public:
	void Initialize();
	void Shutdown() override;

	void Render() override;

	void MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

private:
	HRESULT CreateBitmapFromFile(const std::wstring& path, ID2D1Bitmap1** outBitmap);
};