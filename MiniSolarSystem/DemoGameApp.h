#pragma once

#include "../D2DEngineLib/WinApp.h"

class Camera;
class SpaceObject;

class DemoGameApp :
	public WinApp
{
private:
	Microsoft::WRL::ComPtr<IWICImagingFactory> m_wicImagingFactory;

	Camera* m_camera;
	SpaceObject* m_sun;
	SpaceObject* m_moon;
	SpaceObject* m_earth;

public:
	DemoGameApp() = default;
	~DemoGameApp() = default;

public:
	void Initialize();
	void Shutdown() override;

	void Update() override;
	void Render() override;

	void MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

private:
	HRESULT CreateBitmapFromFile(const std::wstring& path, ID2D1Bitmap1** outBitmap);
};